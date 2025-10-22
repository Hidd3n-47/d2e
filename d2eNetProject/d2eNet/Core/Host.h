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
    uint16_t port = 6666;
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

    void AddPacketToSendToClient(const uint32_t id, Packet& packet) { std::lock_guard lock(mPacketsToSendToClientMutex); mClientIdToPacketsToSend[id].emplace(std::move(packet)); }
    void AddPacketToBroadcast(Packet& packet) { std::lock_guard lock(mPacketsToBroadcastMutex); mPacketsToBroadcast.emplace(std::move(packet)); }

    void SendPackets();

    [[nodiscard]] uint32_t GetNumJoinedClients() const { return mNumJoinedClients; }

    static constexpr uint32_t NUMBER_OF_ALLOWED_CLIENTS = 2;
private:
    ENetHost*   mHost;
    ENetAddress mAddress;
    std::atomic<bool> mRunning;

    uint32_t mNumJoinedClients{ 0 };
    std::unordered_map<uint32_t, uint32_t> mHostAddressToId;
    std::unordered_map<uint32_t, ENetPeer*> mClientIdToPeer;

    std::unordered_map<uint32_t, std::queue<Packet>> mClientIdToPacketsToSend;
    std::queue<Packet> mPacketsReceived;
    std::queue<Packet> mPacketsToBroadcast;

    std::thread mHostThread;
    std::mutex  mPacketsToSendToClientMutex;
    std::mutex  mPacketsReceivedMutex;
    std::mutex  mPacketsToBroadcastMutex;

    std::function<void(uint32_t)> mOnClientConnectedCallback;
    std::function<void(uint32_t)> mOnClientDisconnectedCallback;

    void BroadcastPacket(const Packet& packet) const;
    void SendPacket(const uint32_t id, const Packet& packet);
};

} // Namespace d2eNet.