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

    const d2eNet::HostInitInfo hostInfo
    {
        .ip1 = static_cast<uint8_t>(ip1),
        .ip2 = static_cast<uint8_t>(ip2),
        .ip3 = static_cast<uint8_t>(ip3),
        .ip4 = static_cast<uint8_t>(ip4),
        .onConnectCallback = [&](const uint32_t id) { OnClientConnected(id); },
        .onDisconnectCallback = [&](const uint32_t id) { OnClientDisconnected(id); }
    };
    if (!mHost->Init(hostInfo))
    {
        mLog.Error("Failed to start up host at the given IP: {}.{}.{}.{}:{}", ip1, ip2, ip3, ip4, hostInfo.port);
        return false;
    }
    mLog.Debug("Host running at given IP: {}.{}.{}.{}:{}", ip1, ip2, ip3, ip4,hostInfo.port);

    mLog.Debug("Server initialized.");
    return true;
}

void ServerManager::Run()
{
    mLog.Debug("Server started.");

    // todo need to handle a case of the game being simulated but then player being disconnected.

    while (mServerRunning)
    {
        d2e::Engine::Instance()->StartFrame();

        // Allow the server to be shutdown with the 'Esc' key.
        if (_kbhit())
        {
            if (_getch() == VK_ESCAPE)
            {
                mServerRunning = false;
            }
        }

        d2e::WeakRef<d2e::Scene> activeScene = d2e::Engine::Instance()->GetActiveScene();

        ProcessIncomingPackets(activeScene);

        d2e::Engine::Instance()->Update();
        d2e::Engine::Instance()->PostUpdate();

        SendPacketsToClients(activeScene);

        d2e::Engine::Instance()->EndFrame();
    }
}

void ServerManager::Destroy() const
{
    d2eNet::d2eNet::Destroy();

    mLog.Debug("Server Destroyed.");
}

void ServerManager::ProcessIncomingPackets(d2e::WeakRef<d2e::Scene> activeScene)
{
    // If we don't have a valid scene, we shouldn't process the packets.
    if (!activeScene.IsRefValid())
    {
        return;
    }

    std::optional<d2eNet::Packet> packet = mHost->GetPacket();
    while (packet)
    {
        //mLog.Info("Processed Packet: {}", std::string{ packet->BufBegin(), packet->BufEnd() });
        for (d2eNet::Packet::Iterator it = packet->Begin(); it != packet->End(); ++it)
        {
            const std::string packetString = it.GetPacketLineString();

            switch (it.GetPacketLineType())
            {
            case d2eNet::PacketLineType::ADD_COMPONENT:
            {
                const size_t firstDelimiter  = packetString.find(d2e::SerializeUtils::DELIMITER);
                const size_t secondDelimiter = packetString.find(d2e::SerializeUtils::DELIMITER, firstDelimiter + 1);

                const uint64_t id = std::stoull(packetString.substr(0, firstDelimiter));
                const std::string componentName  = packetString.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1);
                const std::string componentValue = packetString.substr(secondDelimiter + 1);

                d2e::WeakRef<d2e::GameObject> gameObject = d2e::Engine::Instance()->GetActiveScene()->GetGameObject(d2e::Ulid{ id });
                gameObject->AddComponent(componentName)->Deserialize(componentValue);

                mLog.Debug("Added Component [{}] to game object with ID: {} | <{}>", componentName, id, componentValue);
                break;
            }
            case d2eNet::PacketLineType::ADD_GAME_OBJECT:
            {
                uint64_t id;
                d2e::SerializeUtils::Deserialize(id, packetString);

                activeScene->CreateGameObject()->SetId(d2e::Ulid{ id });

                mLog.Debug("Created game object (ID: {})", packetString, id);
                break;
            }
            case d2eNet::PacketLineType::SYNC_GAME_OBJECT_ACROSS_NETWORK:
            {
                mGameObjectsToSyncAcrossNetwork.emplace_back(std::stoull(packetString));
                mLog.Debug("Registered game object (ID: {}) to be synced across networks.", packetString);
                break;
            }
            case d2eNet::PacketLineType::UPDATE_COMPONENT:
            {
                const size_t firstDelimiter  = packetString.find(d2e::SerializeUtils::DELIMITER);
                const size_t secondDelimiter = packetString.find(d2e::SerializeUtils::DELIMITER, firstDelimiter + 1);

                const uint64_t id = std::stoull(packetString.substr(0, firstDelimiter));
                const std::string componentName  = packetString.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1);
                const std::string componentValue = packetString.substr(secondDelimiter + 1);

                activeScene->GetGameObject(d2e::Ulid{ id })->GetComponent(componentName)->Deserialize(componentValue);

                if (componentName == d2e::RigidBody::GetNameStatic())
                    mLog.Warn("Updated Component [{}] to game object with ID: {} | <{}>", componentName, id, componentValue);
                break;
            }
            case d2eNet::PacketLineType::SERVER_HANDLED_PACKET_CONFIRM:
            {
                d2eNet::Packet p;
                p.AddStringToPacket(d2eNet::PacketLineType::SERVER_HANDLED_PACKET_CONFIRM, packetString);
                mHost->AddPacketToBroadcast(p);
                break;
            }
            case d2eNet::PacketLineType::PLAYER_TWO_JOINED:
            default:
                mLog.Warn("Received packet that is not processed: <{}>", packetString);
                break;
            }
        }

        packet = mHost->GetPacket();
    }
}

void ServerManager::SendPacketsToClients(d2e::WeakRef<d2e::Scene> activeScene) const
{
    // If we don't have a valid scene, we shouldn't send the packets.
    if (!activeScene.IsRefValid())
    {
        return;
    }

    for (const d2e::Ulid id : mGameObjectsToSyncAcrossNetwork)
    {
        d2e::WeakRef<d2e::GameObject> gameObject = activeScene->GetGameObject(id);

        d2eNet::Packet packet{ false };

        for (const d2e::IComponent* comp : gameObject->GetComponents())
        {
            //std::cout << "Component <" + comp->GetName() + "> " + comp->Serialize() << std::endl;
            if (comp->SyncValuesOnUpdate())
            {
                packet.UpdateType(id, comp->GetName(), comp->Serialize());
            }
        }

        mHost->AddPacketToBroadcast(packet);
    }
}

void ServerManager::OnClientConnected(const uint32_t id)
{
    mLog.Debug("Client with ID '{}' connected.", id);

    // This is the first player to join the server.
    if (mGameScene == nullptr)
    {
        mLog.Debug("Created game scene.");
        mGameScene = new d2e::Scene();
        d2e::Engine::Instance()->ChangeActiveScene(mGameScene);
        return;
    }

    // The second player has joined the server.
    d2eNet::Packet p;
    p.AddLineWithId(0, d2eNet::PacketLineType::PLAYER_TWO_JOINED);
    mHost->AddPacketToBroadcast(p);
}

void ServerManager::OnClientDisconnected(const uint32_t id)
{
    mLog.Debug("Client with ID '{}' disconnected.", id);

    if (mHost->GetNumJoinedClients() == 0)
    {
        mLog.Debug("Destroyed game scene as no more players remain.");
        mGameScene = nullptr;
        d2e::Engine::Instance()->ChangeActiveScene(mGameScene);
        mGameObjectsToSyncAcrossNetwork.clear();
    }
}

} // Namespace d2eServer.