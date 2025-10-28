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

    /**
     * @brief Initialize the connection.
     * @param info The information needed to start the host.
     * @return Returns \c true if the connection was created, \c false otherwise.
     */
    [[nodiscard]] bool Init(const HostInitInfo& info);

    /**
     * @brief Update creates the connection to the clients and tries to receive information from them.
     * @param timeout The duration that this connection and information receiving occurs.
     */
    void Update(const uint32_t timeout);

    std::optional<Packet> GetPacket();

    /**
     * @brief Add a packet that should be sent to \b all the clients when it next broadcasts packets.
     * @note Broadcasting packets happens on a different thread, and so this won't be instant.
     * @param packet The packet that will be sent to all the clients.
     */
    void AddPacketToBroadcast(Packet& packet) { std::lock_guard lock(mPacketsToBroadcastMutex); mPacketsToBroadcast.emplace(std::move(packet)); }
    /**
     * @brief Broadcast the packets that have been queued for sending.
     */
    void BroadcastPackets();

    /**
     * @brief Get the active number of clients connected to the host.
     * @return The active number of clients connected.
     */
    [[nodiscard]] uint32_t GetNumJoinedClients() const { return mNumJoinedClients; }

    static constexpr uint32_t NUMBER_OF_ALLOWED_CLIENTS = 2;
private:
    ENetHost*   mHost = nullptr;
    ENetAddress mAddress{};

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

    /**
     * @brief A helper function to broadcast a packet via enet to the clients.
     * @param packet The packet that is being sent to the clients.
     */
    void BroadcastPacket(const Packet& packet) const;
};

} // Namespace d2eNet.