#pragma once

#include <mutex>
#include <thread>

#include <queue>
#include <cstdint>
#include <functional>
#include <optional>

#include <enet/enet.h>

#include "d2eNet/Core/Packet.h"

namespace d2eNet
{

struct HostInitInfo
{
    uint8_t ip1, ip2, ip3, ip4;
    uint16_t port = 7777;
    uint32_t timeout = 5;
    std::function<void(uint32_t)> onConnectCallback;
    std::function<void(uint32_t)> onDisconnectCallback;
};

class Host
{
public:
    Host() = default;
    ~Host();

    [[nodiscard]] bool Init(const HostInitInfo& info);

    void Update(const uint32_t timeout);

    std::optional<Packet> GetPacket();

    void AddPacketToBroadcast(Packet& packet) { std::lock_guard lock(mPacketsToBroadcastMutex); mPacketsToBroadcast.emplace(std::move(packet)); }
    void BroadcastPackets();

    [[nodiscard]] uint32_t GetNumJoinedClients() const { return mNumJoinedClients; }

    static constexpr uint32_t NUMBER_OF_ALLOWED_CLIENTS = 2;
private:
    ENetHost*   mHost;
    ENetAddress mAddress;
    std::atomic<bool> mRunning;

    uint32_t mNumJoinedClients{ 0 };
    std::unordered_map<uint32_t, uint32_t> mHostAddressToId;

    std::queue<Packet> mPacketsReceived;
    std::queue<Packet> mPacketsToBroadcast;

    std::thread mHostThread;
    std::mutex  mPacketsReceivedMutex;
    std::mutex  mPacketsToBroadcastMutex;

    std::function<void(uint32_t)> mOnClientConnectedCallback;
    std::function<void(uint32_t)> mOnClientDisconnectedCallback;

    void BroadcastPacket(const Packet& packet) const;
};

} // Namespace d2eNet.