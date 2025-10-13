#pragma once

#include <memory>

#include <Log/Log.h>

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

    [[nodiscard]] inline d2e::Log& Log() { return mLog; }
private:
    static std::unique_ptr<ServerManager> mInstance;

    d2e::Log mLog{ "d2e Game Server" };

    bool mServerRunning = true;

    std::unique_ptr<d2eNet::Host> mHost;

    uint32_t mNumClientsConnected{ 0 };
    void ClientConnected();
};

} // Namespace d2eServer.