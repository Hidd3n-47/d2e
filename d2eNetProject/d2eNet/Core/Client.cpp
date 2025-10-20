#include "Client.h"

#include <format>
#include <iostream>

#include "Packet.h"

namespace d2eNet
{

Client::~Client()
{
    enet_host_destroy(mClient);
}

bool Client::Init(const uint8_t ip1, const uint8_t  ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port)
{
    const std::string ip = std::format("{}.{}.{}.{}", ip1, ip2, ip3, ip4);

    mClient = enet_host_create(nullptr, 1, 2, 0, 0);

    ENetAddress address { .host = ENET_HOST_ANY, .port = 7777 };

    enet_address_set_host(&address, ip.c_str());
    mPeer = enet_host_connect(mClient, &address, 1, 0);

    return mClient && mPeer;
}

void Client::Update(const uint32_t timeout)
{
    ENetEvent event;

    while (enet_host_service(mClient, &event, timeout) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            mConnected = true;
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            mConnected = false;
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            mPacketsReceived.emplace(event.packet->data, event.packet->dataLength);
            enet_packet_destroy(event.packet);
            break;
        }

        //if (event.type == ENET_EVENT_TYPE_CONNECT)
        //{
        //    // todo add.
        //    printf("A new client connected from %x:%u.\n",
        //        event.peer->address.host,
        //        event.peer->address.port);
        //    mConnected = true;
        //    //return;
        //}

        //if (event.type == ENET_EVENT_TYPE_DISCONNECT)
        //{
        //    // todo add.
        //    mConnected = false;
        //    //return;
        //}

        //if (event.type == ENET_EVENT_TYPE_RECEIVE)
        //{
        //    // todo add some info here.
        //        /*printf("A packet of length %u containing %s was received from %s on channel %u from host.\n",
        //            event.packet->dataLength,
        //            reinterpret_cast<const char*>(event.packet->data),
        //            event.peer->data,
        //            event.channelID);*/
        //    mPacketsReceived.emplace(event.packet->data, event.packet->dataLength);
        //    enet_packet_destroy(event.packet);
        //    //return;
        //}
    }
}

std::optional<Packet> Client::GetPacketReceived()
{
    if (mPacketsReceived.empty())
    {
        return {};
    }

    Packet front = mPacketsReceived.front(); mPacketsReceived.pop();
    return { front };
}

void Client::SendPackets()
{
    if (!mConnected)
    {
        return;
    }

    while (!mPacketsToSend.empty())
    {
        const Packet packet = mPacketsToSend.front();
        //std::cout << "Trying to send packet of: <" + std::string{ packet.BufBegin(), packet.BufEnd() } + ">\n";
        SendPacket(packet.GetData(), packet.GetCount(), packet.IsReliable());
        mPacketsToSend.pop();
    }
}

void Client::SendPacket(const void* data, const uint32_t count, const bool reliable) const
{
    ENetPacket* packet{ enet_packet_create(data, count, reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT) };

    enet_peer_send(mPeer, 0, packet);
}

} // Namespace d2eNet.

