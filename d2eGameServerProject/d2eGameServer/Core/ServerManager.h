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

    bool Init();
    void Run();
    void Destroy();

    void ProcessIncomingPackets(d2e::WeakRef<d2e::Scene> activeScene);
    void SendPacketsToClients(d2e::WeakRef<d2e::Scene> activeScene) const;

    void OnClientConnected(const uint32_t id);
    void OnClientDisconnected(const uint32_t id);

    [[nodiscard]] inline d2e::Log& Log() { return mLog; }
private:
    static std::unique_ptr<ServerManager> mInstance;

    d2e::Log mLog{ "d2e Game Server" };

    bool mServerRunning = true;

    d2e::Scene* mGameScene = nullptr;

    std::unique_ptr<d2eNet::Host> mHost;

    std::vector<std::pair<uint32_t, d2e::Ulid>> mGameObjectsToSyncAcrossNetwork;
};

} // Namespace d2eServer.