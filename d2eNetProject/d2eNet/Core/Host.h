#pragma once

#include <mutex>
#include <thread>

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

    [[nodiscard]] bool Init(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port, const uint32_t timeout = 5);

    void Update(const uint32_t timeout);

    std::optional<Packet> GetPacket();

    void AddPacketToBroadcast(Packet& packet) { std::lock_guard lock(mPacketsToBroadcastMutex); mPacketsToBroadcast.emplace(std::move(packet)); }
    void BroadcastPackets();

    [[nodiscard]] uint16_t GetNumJoinedClients() const { return mNumJoinedClients; }

    static constexpr uint16_t NUMBER_OF_ALLOWED_CLIENTS = 2;
private:
    ENetHost*   mHost;
    ENetAddress mAddress;
    std::atomic<bool> mRunning;

    uint16_t mNumJoinedClients{ 0 };

    std::queue<Packet> mPacketsReceived;
    std::queue<Packet> mPacketsToBroadcast;

    std::thread mHostThread;
    std::mutex  mPacketsReceivedMutex;
    std::mutex  mPacketsToBroadcastMutex;

    void BroadcastPacket(const Packet& packet) const;
};

} // Namespace d2eNet.