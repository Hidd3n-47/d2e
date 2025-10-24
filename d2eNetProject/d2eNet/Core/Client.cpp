#include "Client.h"

#include <format>

#include "Packet.h"

namespace d2eNet
{

Client::~Client()
{
    mConnected = false;
    if (mClientThread.joinable())
    {
        mClientThread.join();
    }

    if (mPeer)
    {
        enet_peer_disconnect_now(mPeer, 0);
    }
    if (mClient)
    {
        enet_host_destroy(mClient);
    }

}

bool Client::Init(const uint8_t ip1, const uint8_t  ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port, const uint32_t timeout)
{
    const std::string ip = std::format("{}.{}.{}.{}", ip1, ip2, ip3, ip4);

    mClient = enet_host_create(nullptr, 1, 2, 0, 0);

    ENetAddress address { .host = ENET_HOST_ANY, .port = 7777 };

    enet_address_set_host(&address, ip.c_str());
    mPeer = enet_host_connect(mClient, &address, 1, 0);

    bool setId = false;
    ENetEvent event;
    while (enet_host_service(mClient, &event, 500) > 0 && !setId)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            mConnected = true;
            break;
        case ENET_EVENT_TYPE_RECEIVE:
        {
            const std::string idStr{ reinterpret_cast<char*>(event.packet->data) };
            mClientId = static_cast<uint32_t>(std::stoi(idStr));
            enet_packet_destroy(event.packet);

            setId = true;
            break;
        }
        }
    }

    const bool initOkay = mClient && mPeer;

    if (initOkay)
    {
        mClientThread = std::thread(&Client::Update, this, timeout);
    }

    return initOkay;
}

void Client::Update(const uint32_t timeout)
{
    while (mConnected)
    {
        ENetEvent event;

        while (enet_host_service(mClient, &event, timeout) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_DISCONNECT:
                mConnected = false;
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

        SendPackets();
    }
}

std::optional<Packet> Client::GetPacketReceived()
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

void Client::ServerProcessedPacketsConfirmation(const std::function<void()>& onServerPacketHandled)
{
    Packet handledPacket;
    handledPacket.AddStringToPacket(PacketLineType::SERVER_HANDLED_PACKET_CONFIRM, std::to_string(mServerConfirmationId));
    AddPacketToSend(handledPacket);

    mPacketIdToHandledCallback[mServerConfirmationId++] = onServerPacketHandled;
}

void Client::ServerHandledPacketQuery(const uint32_t handledPacketId)
{
    if (const auto it = mPacketIdToHandledCallback.find(handledPacketId); it != mPacketIdToHandledCallback.end())
    {
        mPacketIdToHandledCallback[handledPacketId]();
        mPacketIdToHandledCallback.erase(it);
    }
}

void Client::SendPackets()
{
    if (!mConnected)
    {
        return;
    }

    std::queue<Packet> localQueue;
    {
        std::lock_guard lock(mPacketsToSendMutex);
        std::swap(localQueue, mPacketsToSend);
    }

    while (!localQueue.empty())
    {
        SendPacket(localQueue.front());
        localQueue.pop();
    }

    enet_host_flush(mClient);
}

void Client::SendPacket(const Packet& packet) const
{
    ENetPacket* enetPacket{ enet_packet_create(packet.GetData(), packet.GetCount(), packet.IsReliable() ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT) };

    enet_peer_send(mPeer, 0, enetPacket);
}

} // Namespace d2eNet.

