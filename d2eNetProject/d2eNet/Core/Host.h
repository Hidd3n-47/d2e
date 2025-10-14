#pragma once

#include <queue>
#include <cstdint>
#include <optional>

#include <enet/enet.h>

#include "d2eNet/Core/Packet.h"

namespace d2eNet
{

class Host
{
public:
    Host() = default;
    ~Host();

    [[nodiscard]] bool Init(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port, const bool host = true);

    void Update(const uint32_t timeout);

    std::optional<Packet> GetPacket() { if (mPacketsReceived.empty()) return {}; Packet front = mPacketsReceived.front(); mPacketsReceived.pop(); return { front }; }
    void ProcessPackets();

    [[nodiscard]] uint16_t GetNumJoinedClients() const { return mNumJoinedClients; }

    static constexpr uint16_t NUMBER_OF_ALLOWED_CLIENTS = 1;
private:
    ENetHost*   mHost;
    ENetAddress mAddress;

    uint16_t mNumJoinedClients{ 0 };

    std::queue<Packet> mPacketsReceived;
};

} // Namespace d2eNet.