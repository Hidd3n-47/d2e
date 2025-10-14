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

    mClient = enet_host_create(nullptr, 1, 1, 0, 0);

    ENetAddress address { .port = 7777 };
    enet_address_set_host(&address, ip.c_str());
    mPeer = enet_host_connect(mClient, &address, 1, 0);

    return mClient && mPeer;
}

void Client::Update(const uint32_t timeout) const
{
    ENetEvent event;

    while (enet_host_service(mClient, &event, timeout) > 0)
    {
        if (event.type == ENET_EVENT_TYPE_RECEIVE)
        {
            // todo add some info here.

            return;
        }

        if (event.type == ENET_EVENT_TYPE_CONNECT)
        {
            // todo add.
            printf("A new client connected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);

            return;
        }

        if (event.type == ENET_EVENT_TYPE_DISCONNECT)
        {
            // todo add.

            return;
        }
    }
}

void Client::AddPacketToSend(Packet& packet)
{
    mPacketsToSend.emplace(std::move(packet));
}

void Client::SendPackets()
{
    while (!mPacketsToSend.empty())
    {
        const Packet packet = mPacketsToSend.front();
        SendPacket(packet.GetData(), packet.GetCount());
        mPacketsToSend.pop();
    }
}

void Client::SendPacket(const char* data) const
{
    std::cout << "Trying to send packet of: <" + std::string{ data } + ">\n";
    //todo look at where we destroy this packet.
    ENetPacket* packet{ enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE) };

    enet_peer_send(mPeer, 0, packet);
}

void Client::SendPacket(const void* data, const uint32_t count) const
{
    //std::cout << "Trying to send packet of: <" + std::string{ data } + ">\n";
    //todo look at where we destroy this packet.
    ENetPacket* packet{ enet_packet_create(data, count, ENET_PACKET_FLAG_RELIABLE) };

    enet_peer_send(mPeer, 0, packet);
}

} // Namespace d2eNet.

