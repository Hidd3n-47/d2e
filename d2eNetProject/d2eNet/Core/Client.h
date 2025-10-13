#pragma once

#include <queue>
#include <cstdint>

#include <enet/enet.h>

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

    //void AddPacketToSend(Packet&& packet);

    void SendPacket(const char* data) const;
private:
    ENetHost* mClient;
    ENetPeer* mPeer;

    //std::queue<Packet> mPacketsToSend;
};

} // Namespace d2eNet.