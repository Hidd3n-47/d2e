#pragma once

#include <queue>
#include <cstdint>

#include <enet/enet.h>

#include "d2eNet/Core/Packet.h"

namespace d2eNet
{

class Packet;

class Client
{
public:
    Client() = default;
    ~Client();

    [[nodiscard]] bool Init(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port);

    void Update(const uint32_t timeout) const;

    void AddPacketToSend(Packet& packet);

    void SendPackets();
    void SendPacket(const char* data) const;
    void SendPacket(const void* data, const uint32_t count) const;
private:
    ENetHost* mClient;
    ENetPeer* mPeer;

    std::queue<Packet> mPacketsToSend;
};

} // Namespace d2eNet.