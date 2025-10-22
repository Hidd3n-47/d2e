#pragma once

#include <queue>
#include <cstdint>
#include <optional>
#include <functional>

#include <mutex>
#include <thread>

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

    [[nodiscard]] bool Init(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port, const uint32_t timeout = 5);

    void Update(const uint32_t timeout);

    inline void AddPacketToSend(Packet& packet)   { std::lock_guard lock(mPacketsToSendMutex); mPacketsToSend.emplace(std::move(packet)); }
    inline void AddPacketReceived(Packet& packet) { std::lock_guard lock(mPacketsReceivedMutex); mPacketsReceived.emplace(std::move(packet)); }

    std::optional<Packet> GetPacketReceived();

    void ServerProcessedPacketsConfirmation(const std::function<void()>& onServerPacketHandled);
    void ServerHandledPacketQuery(const uint32_t handledPacketId);

    void SendPackets();
    void SendPacket(const Packet& packet) const;

    [[nodiscard]] inline uint16_t GetId() const { return mClientId; }
private:
    ENetHost* mClient;
    ENetPeer* mPeer;
    std::atomic<bool> mConnected;
    uint16_t mClientId;

    std::queue<Packet> mPacketsToSend;
    std::queue<Packet> mPacketsReceived;

    std::thread mClientThread;
    std::mutex mPacketsToSendMutex;
    std::mutex mPacketsReceivedMutex;

    uint32_t mServerConfirmationId{ 0 };
    std::unordered_map<uint32_t, std::function<void()>> mPacketIdToHandledCallback;
};

} // Namespace d2eNet.