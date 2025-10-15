#pragma once

#include <queue>
#include <cstdint>
#include <optional>

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

    void Update(const uint32_t timeout);

    inline void AddPacketToSend(Packet& packet)   { mPacketsToSend.emplace(std::move(packet)); }
    inline void AddPacketReceived(Packet& packet) { mPacketsReceived.emplace(std::move(packet)); }

    std::optional<Packet> GetPacketReceived();

    void SendPackets();
    void SendPacket(const void* data, const uint32_t count) const;
private:
    ENetHost* mClient;
    ENetPeer* mPeer;

    std::queue<Packet> mPacketsToSend;
    std::queue<Packet> mPacketsReceived;
};

} // Namespace d2eNet.