#include "ServerManager.h"

#include <conio.h>
#include <iostream>

#include <d2eNet/Core/Host.h>
#include <d2eNet/Core/d2eNet.h>

#include <d2e/ES/Scene.h>
#include <d2e/Core/Rtti.h>
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

        if (mHost->GetNumJoinedClients() != mNumClientsConnected)
        {
            ClientConnected();
        }

        std::optional<d2eNet::Packet> p = mHost->GetPacket();
        while (p)
        {
            for (d2eNet::Packet::Iterator it = p->Begin(); it != p->End(); ++it)
            {
                const std::string packetString = it.GetPacketLineString();

                switch (it.GetPacketLineType())
                {
                case d2eNet::PacketLineType::ADD_COMPONENT:
                {
                    const size_t firstDelimiter  = packetString.find('|');
                    const size_t secondDelimiter = packetString.find('|', firstDelimiter + 1);

                    const uint32_t id = std::stoul(packetString.substr(0, firstDelimiter));
                    const std::string componentName  = packetString.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1);
                    const std::string componentValue = packetString.substr(secondDelimiter + 1);

                    d2e::WeakRef<d2e::GameObject> gameObject = d2e::Engine::Instance()->GetActiveScene()->GetGameObject(id);
                    d2e::WeakRef<d2e::IComponent> component  = gameObject->AddComponent(componentName);
                    component->Deserialize(componentValue);

                    mLog.Debug("Added Component [{}] to game object with ID: {} | <{}>", componentName, id, componentValue);
                    break;
                }
                case d2eNet::PacketLineType::ADD_GAME_OBJECT:
                {
                    uint32_t id;
                    d2e::SerializeUtils::Deserialize(id, packetString);

                    d2e::WeakRef<d2e::GameObject> go = d2e::Engine::Instance()->GetActiveScene()->CreateGameObject();
                    go->SetId(id);

                    mLog.Debug("Created game object (ID: {})", packetString, id);
                    break;
                }
                default:
                    mLog.Warn("Received packet that is not processed: <{}>", packetString);
                    break;
                }
            }

            p = mHost->GetPacket();
        }

        // Simulate game.
        d2e::Engine::Instance()->Update();

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