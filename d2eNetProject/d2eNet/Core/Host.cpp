#include "Host.h"

#include <iostream>
#include <format>

namespace d2eNet
{

Host::~Host()
{
    enet_host_destroy(mHost);
}

bool Host::Init(const uint8_t ip1, const uint8_t  ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port, const bool host)
{
    const std::string ip = std::format("{}.{}.{}.{}", ip1, ip2, ip3, ip4);

    if (host)
    {
        enet_address_set_host(&mAddress, ip.c_str());
        mAddress.port = port;

        mHost = enet_host_create(
            &mAddress,
            NUMBER_OF_ALLOWED_CLIENTS /* allow up to 32 clients and/or outgoing connections */,
            1 /* allow up to 2 channels to be used, 0 and 1 */,
            0,
            0);
    }
    else
    {
        mHost = enet_host_create(nullptr, 1, 1, 0, 0);
    }

    return mHost;
}

void Host::Update(const uint32_t timeout)
{
    ENetEvent event;

    while (enet_host_service(mHost, &event, timeout) > 0)
    {
        if (event.type == ENET_EVENT_TYPE_CONNECT)
        {
            // todo add.
            ++mNumJoinedClients;
            printf("A new client connected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);
            return;
        }

        if (event.type == ENET_EVENT_TYPE_DISCONNECT)
        {
            // todo add.
            --mNumJoinedClients;
            printf("A new client disconnected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);

            return;
        }

        if (event.type == ENET_EVENT_TYPE_RECEIVE)
        {
            // todo add some info here.
            /*printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                event.packet->dataLength,
                reinterpret_cast<const char*>(event.packet->data),
                event.peer->data,
                event.channelID);*/

                //mPacketsReceived.emplace(std::string{ reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength });
            mPacketsReceived.emplace(event.packet->data, event.packet->dataLength);

            return;
        }
    }
}

void Host::BroadcastPacket(const Packet& packet) const
{
    //std::cout << "Trying to send packet of: <" + std::string{ packet.BufBegin(), packet.BufEnd() } + ">\n";
    ENetPacket* enetPacket{ enet_packet_create(packet.GetData(), packet.GetCount(), packet.IsReliable() ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT) };

    enet_host_broadcast(mHost, 0, enetPacket);
}

} // Namespace d2eNet.

