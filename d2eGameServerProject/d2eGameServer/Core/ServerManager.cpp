#include "ServerManager.h"

#include <iostream>

#include <enet/enet.h>

#include <d2eNet/Core/Host.h>
#include <d2eNet/Core/d2eNet.h>

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

    while (true)
    {
        mHost->Update();
    }

    // Wait for clients to join server.
    while (mHost->GetNumJoinedClients() < d2eNet::Host::NUMBER_OF_ALLOWED_CLIENTS)
    {
    }

    // Since clients have joined, simulate the game.
    while (true)
    {
        
    }
}

void ServerManager::Destroy()
{
    d2eNet::d2eNet::Destroy();

    mLog.Debug("Server Destroyed.");
}

} // Namespace d2eServer.