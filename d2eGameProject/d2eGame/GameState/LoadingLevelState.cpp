#include "LoadingLevelState.h"

#include <d2e/ES/Scene.h>
#include <d2e/Core/Engine.h>

#include <d2e/ES/Components/Tag.h>
#include <d2e/ES/Components/Transform.h>
#include <d2e/ES/Components/BattleTimer.h>
#include <d2e/ES/Components/PingDisplay.h>
#include <d2e/ES/Components/RectangleSprite.h>
#include <d2e/ES/Components/StaticBoxCollider.h>

#include <d2eNet/Core/Packet.h>
#include <d2eNet/Core/Client.h>

#include "Scene/GameScene.h"
#include "src/Defines.h"

namespace d2eGame
{

void LoadingLevelState::Init(d2e::WeakRef<d2e::Scene> scene)
{
    mParent = scene;

    const d2e::Vec2 windowSize = d2e::Engine::Instance()->GetWindowSize();

    d2e::WeakRef<d2eNet::Client> client = d2e::Engine::Instance()->GetClient();
    bool uploadLevelData = client->GetId() == 1;

    // Testing floor.
    {
        d2eNet::Packet packet;

        d2e::WeakRef<d2e::GameObject> floorObject = scene->CreateGameObject();
        const d2e::Ulid id = floorObject->GetId();
        packet.AddLineWithId(id);

        d2e::WeakRef<d2e::RectangleSprite> floorSprite = floorObject->AddComponent<d2e::RectangleSprite>();
        floorSprite->SetHalfExtents(windowSize * d2e::Vec2{ 0.2f, 0.05f });
        floorSprite->SetColor(sf::Color::Blue);
        packet.AddType<d2e::RectangleSprite>(id, floorSprite->Serialize());

        d2e::WeakRef<d2e::Transform> transform = floorObject->GetComponent<d2e::Transform>();
        transform->translation = windowSize * d2e::Vec2{ 0.5f, 0.5f };
        packet.AddType<d2e::Transform>(id, transform->Serialize());

        d2e::WeakRef<d2e::StaticBoxCollider> bc = floorObject->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(floorSprite->GetHalfExtents());
        packet.AddType<d2e::StaticBoxCollider>(id, bc->Serialize());

        if (uploadLevelData)
        {
            client->AddPacketToSend(packet);
        }
    }

    // Wall boundaries.
    CreateWall(client, windowSize * d2e::Vec2{ 0.0f, 0.5f }, d2e::Vec2{ 5.0f, windowSize.y });
    CreateWall(client, windowSize * d2e::Vec2{ 1.0f, 0.5f }, d2e::Vec2{ 5.0f, windowSize.y });
    CreateWall(client, windowSize * d2e::Vec2{ 0.5f, 0.0f }, d2e::Vec2{ windowSize.x, 5.0f });
    CreateWall(client, windowSize * d2e::Vec2{ 0.5f, 1.0f }, d2e::Vec2{ windowSize.x, 5.0f });

    // Ping display.
    {
        d2e::WeakRef<d2e::GameObject> ping = scene->CreateGameObject();
        constexpr d2e::Ulid id = d2e::Engine::PING_DISPLAY_ULID;
        ping->SetId(id);

        d2e::WeakRef<d2e::PingDisplay> pingDisplay = ping->AddComponent<d2e::PingDisplay>();
        pingDisplay->SetSyncValuesOnUpdate(true);

        d2e::WeakRef<d2e::Transform> transform = ping->GetComponent<d2e::Transform>();
        transform->translation = windowSize * d2e::Vec2{ 0.015f, 0.015f };

        if (client->GetId() == 1)
        {
            d2eNet::Packet packet;
            packet.AddLineWithId(id);
            packet.AddType<d2e::PingDisplay>(id, pingDisplay->Serialize());
            packet.AddType<d2e::Transform>(id, transform->Serialize());
            packet.AddSyncObject(id);
            client->AddPacketToSend(packet);
        }
    }

    // Battle timer.
    {
        d2e::WeakRef<d2e::GameObject> ping = scene->CreateGameObject();
        constexpr d2e::Ulid id = d2e::Engine::BATTLE_TIMER_ULID;
        ping->SetId(id);

        d2e::WeakRef<d2e::BattleTimer> timer = ping->AddComponent<d2e::BattleTimer>();
        timer->SetSyncValuesOnUpdate(true);

        d2e::WeakRef<d2e::Transform> transform = ping->GetComponent<d2e::Transform>();
        transform->translation = windowSize * d2e::Vec2{ 0.5f };

        if (client->GetId() == 1)
        {
            d2eNet::Packet packet;
            packet.AddLineWithId(id);
            packet.AddType<d2e::BattleTimer>(id, timer->Serialize());
            packet.AddType<d2e::Transform>(id, transform->Serialize());
            packet.AddSyncObject(id);
            client->AddPacketToSend(packet);
        }
    }

    if (d2e::Engine::Instance()->GetClient()->GetId() != 1)
    {
        d2e::WeakRef<GameScene> gameScene = scene.Cast<GameScene>();
        gameScene->GetPlayer().CreatePrefab(scene, false);
        gameScene->GetOtherPlayer().CreatePrefab(scene, true);
        GAME_LOG("Locally created the players");
    }

    client->ServerProcessedPacketsConfirmation([&] { LoadingCompleted(); });
}

void LoadingLevelState::Update()
{
    if (mLoadingCompleted)
    {
        GAME_LOG("Loading Completed");
        if (d2e::Engine::Instance()->GetClient()->GetId() == 1)
        {
            mParent.Cast<GameScene>()->ChangeGameState(GameState::WAITING_FOR_PLAYERS);
        }
        else
        {
            mParent.Cast<GameScene>()->ChangeGameState(GameState::BATTLE_COUNTDOWN);
        }
    }
}

void LoadingLevelState::CreateWall(d2e::WeakRef<d2eNet::Client> client, const d2e::Vec2 translation, const d2e::Vec2 halfExtents)
{
    d2eNet::Packet packet;

    d2e::WeakRef<d2e::GameObject> wall = mParent->CreateGameObject();
    const d2e::Ulid id = wall->GetId();
    packet.AddLineWithId(id);

    d2e::WeakRef<d2e::Transform> transform = wall->GetComponent<d2e::Transform>();
    transform->translation = translation;
    packet.AddType<d2e::Transform>(id, transform->Serialize());

    d2e::WeakRef<d2e::StaticBoxCollider> bc = wall->AddComponent<d2e::StaticBoxCollider>();
    bc->SetHalfExtents(halfExtents);
    packet.AddType<d2e::StaticBoxCollider>(id, bc->Serialize());

    d2e::WeakRef<d2e::Tag> tag = wall->AddComponent<d2e::Tag>();
    tag->tag = d2e::ComponentTag::WALL;
    packet.AddType<d2e::Tag>(id, tag->Serialize());

    if (client->GetId() == 1)
    {
        client->AddPacketToSend(packet);
    }
}

} // Namespace d2eGame.