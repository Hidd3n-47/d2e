#pragma once

#include <memory>

#include <Log/Log.h>

#include <d2e/Core/WeakRef.h>

namespace d2e
{
class Ulid;
class Scene;
}

namespace d2eNet
{
class Host;
} // Namespace d2eNet.

namespace d2eServer
{

class ServerManager
{
public:
    [[nodiscard]] inline static ServerManager* Instance() { return mInstance.get(); }

    /**
     * @brief Initialize the server and setup resources needed to run the game server.
     * @return Returns \c true when successfully initialized, otherwise \c false.
     */
    bool Init();
    /**
     * @brief The main loop of the server.
     * This should handle processing and sending packets to clients, as well as locally run the game in a headless manner.
     */
    void Run();
    /**
     * @brief Free resources used in the engine.
     */
    void Destroy() const;

    /**
     * @brief Process all the incoming packets from the clients.
     * @param activeScene The currently active scene. Game Objects/Components that are added will be in this scene.
     */
    void ProcessIncomingPackets(d2e::WeakRef<d2e::Scene> activeScene);
    /**
     * @brief Construct and send the packets to the clients of the game objects that are synchronised across the network.
     * @param activeScene The currently active scene of the Game Objects/Components.
     */
    void SendPacketsToClients(d2e::WeakRef<d2e::Scene> activeScene) const;

    /**
     * @brief Function for when a client is connected.
     * @param id The ID of the client that connected.
     */
    void OnClientConnected(const uint32_t id);
    /**
     * @brief Function for when a client is disconnected.
     * @param id The ID of the client that was disconnected.
     */
    void OnClientDisconnected(const uint32_t id);

    [[nodiscard]] inline d2e::Log& Log() { return mLog; }
private:
    static std::unique_ptr<ServerManager> mInstance;

    d2e::Log mLog{ "d2e Game Server" };

    bool mServerRunning = true;

    d2e::Scene* mGameScene = nullptr;

    std::unique_ptr<d2eNet::Host> mHost;

    std::vector<d2e::Ulid> mGameObjectsToSyncAcrossNetwork;
};

} // Namespace d2eServer.