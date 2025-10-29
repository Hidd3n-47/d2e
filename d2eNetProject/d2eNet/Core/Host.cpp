#include "Host.h"

#include <format>

namespace d2eNet
{

Host::~Host()
{
    mRunning = false;
    if (mHostThread.joinable())
    {
        mHostThread.join();
    }

    enet_host_destroy(mHost);
}

bool Host::Init(const HostInitInfo& info)
{
    const std::string ip = std::format("{}.{}.{}.{}", info.ip1, info.ip2, info.ip3, info.ip4);
    if (ip == "0.0.0.0")
    {
        mAddress.host = ENET_HOST_ANY;
    }
    else
    {
        enet_address_set_host(&mAddress, ip.c_str());
    }

    mAddress.port = info.port;

    mHost = enet_host_create(&mAddress, NUMBER_OF_ALLOWED_CLIENTS, 2, 0, 0);

    mRunning    = mHost;
    mHostThread = std::thread(&Host::Update, this, info.timeout);

    mOnClientConnectedCallback    = info.onConnectCallback;
    mOnClientDisconnectedCallback = info.onDisconnectCallback;

    return mRunning;
}

void Host::Update(const uint32_t timeout)
{
    while (mRunning)
    {
        ENetEvent event;

        while (enet_host_service(mHost, &event, timeout) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
            {
                ++mNumJoinedClients;
                if (mOnClientConnectedCallback)
                {
                    mOnClientConnectedCallback(mNumJoinedClients);
                }
                mHostAddressToId[event.peer->address.host] = mNumJoinedClients;

                const std::string idString = std::to_string(mNumJoinedClients);
                const char* idStringCStr = idString.c_str();
                ENetPacket* packet{ enet_packet_create(idStringCStr, strlen(idStringCStr) + 1, ENET_PACKET_FLAG_RELIABLE) };
                enet_peer_send(event.peer, 0, packet);
                enet_host_flush(mHost);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
                --mNumJoinedClients;
                if (mOnClientDisconnectedCallback)
                {
                    mOnClientDisconnectedCallback(mHostAddressToId[event.peer->address.host]);
                }
                mHostAddressToId.erase(event.peer->address.host);
                break;
            case ENET_EVENT_TYPE_RECEIVE:
            {
                std::lock_guard lock(mPacketsReceivedMutex);
                mPacketsReceived.emplace(event.packet->data, event.packet->dataLength);
                enet_packet_destroy(event.packet);
                break;
            }
            }
        }

        BroadcastPackets();
    }
}

std::optional<Packet> Host::GetPacket()
{
    std::lock_guard lock(mPacketsReceivedMutex);

    if (mPacketsReceived.empty())
    {
        return {};
    }

    Packet front = mPacketsReceived.front();
    mPacketsReceived.pop();

    return { front };
}

void Host::BroadcastPackets()
{
    std::queue<Packet> localQueue;
    {
        std::lock_guard lock(mPacketsToBroadcastMutex);
        std::swap(localQueue, mPacketsToBroadcast);
    }

    while (!localQueue.empty())
    {
        BroadcastPacket(localQueue.front());
        localQueue.pop();
    }

    enet_host_flush(mHost);
}


void Host::BroadcastPacket(const Packet& packet) const
{
    ENetPacket* enetPacket{ enet_packet_create(packet.GetData(), packet.GetCount(), packet.IsReliable() ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT) };

    enet_host_broadcast(mHost, 0, enetPacket);
}

} // Namespace d2eNet.

