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

/**
 * @class Client: A client is used to connect to a server/host in order to communicate to and from.
 */
class Client
{
public:
    Client() = default;
    ~Client();

    /**
     * @brief Initialize the connection to the server/host.
     * @param ip1 The first part of the IP address that the client is connecting to  i.e. IP1. X  . X  . X
     * @param ip2 The second part of the IP address that the client is connecting to i.e. X  . IP2. X  . X
     * @param ip3 The third part of the IP address that the client is connecting to  i.e. X  . X  . IP3. X
     * @param ip4 The forth part of the IP address that the client is connecting to  i.e. X  . X  . X  . IP4
     * @param port The port the connection runs through.
     * @param timeout The duration that the client will attempt to connect to the server/host.
     * @return Returns \c true if the connection to the server/host was established, \c false otherwise.
     */
    [[nodiscard]] bool Init(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4, const uint16_t port, const uint32_t timeout = 5);

    /**
     * @brief Update creates the connection to the server/host and tries to receive information from the server.
     * @param timeout The duration that this connection and information receiving occurs.
     */
    void Update(const uint32_t timeout);

    /**
     * @brief Add a packet that should be sent to the server/host when it next sends packets.
     * @note Sending packets happens on a different thread, and so this won't be instant.
     * @param packet The packet that will be sent to the server/host.
     */
    inline void AddPacketToSend(Packet& packet)   { std::lock_guard lock(mPacketsToSendMutex); mPacketsToSend.emplace(std::move(packet)); }

    /**
     * @brief Add a packet that has been received from the server/host to a queue to be processed.
     * @note Receiving packets happens on a different thread, and so the packet isn't processed immediately.
     * @param packet The packet that has been received from the server/host.
     */
    inline void AddPacketReceived(Packet& packet) { std::lock_guard lock(mPacketsReceivedMutex); mPacketsReceived.emplace(std::move(packet)); }

    std::optional<Packet> GetPacketReceived();

    /**
     * @brief A special type of that packet is encoded with an ID that can be used to confirm if a packet has been received by the host/server.<br/>
     *        Once the server receives this packet with the ID, it acknowledges by sending the same ID back. <br/>
     *        Once the ID has been received back from the server on the client side, the callback will be called.
     * @param onServerPacketHandled The callback that is called when the server replies with the packet ID signaling the server acknowledge the packet.
     */
    void ServerProcessedPacketsConfirmation(const std::function<void()>& onServerPacketHandled);

    /**
     * @brief This should be called once the server acknowledges the packet that is sent. The packet will contain the ID, and this ID can be used to call the callback to process the server acknowledgement.
     * @see \c ServerProcessedPacketsConfirmation
     * @param handledPacketId The ID of the packet that has successfully been acknowledged from the server.
     */
    void ServerHandledPacketQuery(const uint32_t handledPacketId);

    /**
     * @brief Send the packets to the server/host that have been queued for sending.
     */
    void SendPackets();

    /**
     * @brief Get the ID of the client. This ID will be set once the client's connection to the server is acknowledged.<br/>
     *        This ID is in increasing order of connection, so a client with ID 1 would have connected to the host before a client with ID 2.
     * @return The ID of the client. A positive value indicates a valid connection.
     */
    [[nodiscard]] inline uint32_t GetId() const { return mClientId; }
private:
    ENetHost* mClient;
    ENetPeer* mPeer;
    uint32_t  mClientId;

    std::atomic<bool> mConnected;

    std::queue<Packet> mPacketsToSend;
    std::queue<Packet> mPacketsReceived;

    std::thread mClientThread;
    std::mutex  mPacketsToSendMutex;
    std::mutex  mPacketsReceivedMutex;

    uint32_t mServerConfirmationId{ 0 };
    std::unordered_map<uint32_t, std::function<void()>> mPacketIdToHandledCallback;

    /**
     * @brief A helper function to send a packet via enet to the server/host.
     * @param packet The packet that is being sent to the server/host.
     */
    void SendPacket(const Packet& packet) const;
};

} // Namespace d2eNet.