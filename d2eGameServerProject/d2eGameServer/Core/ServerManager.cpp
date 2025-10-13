#include "ServerManager.h"

#include <conio.h>
#include <iostream>

#include <d2eNet/Core/Host.h>
#include <d2eNet/Core/d2eNet.h>

#include <d2e/Core/Engine.h>

namespace d2eServer
{

std::unique_ptr<ServerManager> ServerManager::mInstance = std::make_unique<ServerManager>();

bool ServerManager::Init()
{
    if (!d2eNet::d2eNet::Init())
    {
        mLog.Error("Failed to init network, server will not start.");
        return false;
    }

    mLog.Info("Please enter IP for server: ");
    int ip1, ip2, ip3, ip4;
    std::cin >> ip1 >> ip2 >> ip3 >> ip4;

    mHost = std::make_unique<d2eNet::Host>();
    constexpr uint16_t port = 7777;
    if (!mHost->Init(static_cast<uint8_t>(ip1), static_cast<uint8_t>(ip2), static_cast<uint8_t>(ip3), static_cast<uint8_t>(ip4), port))
    {
        mLog.Error("Failed to start up host at the given IP: {}.{}.{}.{}:{}", ip1, ip2, ip3, ip4, port);
        return false;
    }
    mLog.Debug("Host running at given IP: {}.{}.{}.{}:{}", ip1, ip2, ip3, ip4, port);

    mLog.Debug("Server initialized.");
    return true;
}

void ServerManager::Run()
{
    mLog.Debug("Server started.");

    // todo need to handle a case of the game being simulated but then player being disconnected.

    while (mServerRunning)
    {
        // Allow the server to be shutdown with the 'Esc' key.
        if (_kbhit())
        {
            if (_getch() == VK_ESCAPE)
            {
                mServerRunning = false;
            }
        }

        // Receive info.
        mHost->Update(3);

        // Simulate game.

        // Send info.
        // the only thing we should need to send is the player info - specifically the rb and transform component.
    }
}

void ServerManager::Destroy()
{
    d2eNet::d2eNet::Destroy();

    mLog.Debug("Server Destroyed.");
}

void ServerManager::ClientConnected()
{
    ++mNumClientsConnected;

    if (mNumClientsConnected != 1)
    {
        return;
    }

    d2e::WeakRef<d2e::Scene> scene = d2e::Engine::Instance()->GetActiveScene();
    d2e::Engine::Instance()->RemoveScene(scene);

    scene = d2e::Engine::Instance()->CreateScene();
    d2e::Engine::Instance()->SetActiveScene(scene);
}

} // Namespace d2eServer.