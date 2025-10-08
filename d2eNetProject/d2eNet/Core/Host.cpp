#include "Host.h"

#include <format>

namespace d2eNet
{

Host::~Host()
{
    enet_host_destroy(mHost);
}

bool Host::Init(const uint8_t ip1, const uint8_t  ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port)
{
    const std::string ip = std::format("{}.{}.{}.{}", ip1, ip2, ip3, ip4);

    enet_address_set_host(&mAddress, ip.c_str());
    mAddress.port = port;

    mHost = enet_host_create(
        &mAddress,
        NUMBER_OF_ALLOWED_CLIENTS /* allow up to 32 clients and/or outgoing connections */,
        1 /* allow up to 2 channels to be used, 0 and 1 */,
        0,
        0);

    return mHost != nullptr;
}

void Host::Update()
{
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(mHost, &event, 1000) > 0)
    {
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
        }

	        //enet_packet_destroy(event.packet);
	        break;
        }
    }
}

} // Namespace d2eNet.

