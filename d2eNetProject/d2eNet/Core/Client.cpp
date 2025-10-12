#include "Client.h"

#include <format>

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
        return;
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            printf("A new client connected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);

            //e.peer->data = "Client information";
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            printf("%s disconnected.\n", static_cast<char*>(event.peer->data));
            break;
        case ENET_EVENT_TYPE_RECEIVE:
        {
            printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                event.packet->dataLength,
                event.packet->data,
                event.peer->data,
                event.channelID);
            const char* data = "packet received";
            ENetPacket* packet{ enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE) };

            enet_peer_send(event.peer, 0, packet);
            //enet_packet_destroy(event.packet);
            break;
        }

        }
    }
}

void Client::SendPacket(const char* data) const
{
    //todo look at where we destroy this packet.
    ENetPacket* packet{ enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE) };

    enet_peer_send(mPeer, 0, packet);
}

} // Namespace d2eNet.

