#pragma once

#include <cstdint>

#include <enet/enet.h>

namespace d2eNet
{

class Client
{
public:
    Client() = default;
    ~Client();

    [[nodiscard]] bool Init(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port);

    void Update(const uint32_t timeout) const;

    void SendPacket(const char* data) const;
private:
    ENetHost* mClient;
    ENetPeer* mPeer;
};

} // Namespace d2eNet.