#include "LoadingLevelState.h"

#include <d2e/ES/Scene.h>
#include <d2e/Core/Engine.h>

#include <d2e/ES/Components/Tag.h>
#include <d2e/ES/Components/Transform.h>
#include <d2e/ES/Components/BattleTimer.h>
#include <d2e/ES/Components/PingDisplay.h>
#include <d2e/ES/Components/StaticBoxCollider.h>

#include <d2eNet/Core/Packet.h>
#include <d2eNet/Core/Client.h>

#include "d2e/ES/Components/Sprite.h"
#include "Scene/GameScene.h"
#include "src/Defines.h"
#include "Systems/SplatAnimationManager.h"

namespace d2eGame
{

void LoadingLevelState::Init(d2e::WeakRef<d2e::Scene> scene)
{
    mParent = scene;

    d2e::WeakRef<d2eNet::Client> client = d2e::Engine::Instance()->GetClient();
    bool uploadLevelData = client->GetId() == 1;

    // Floors.
    constexpr d2e::Vec2 smallFloorScale{ 0.4f, 0.4f };
    constexpr d2e::Vec2 largeFloorScale{ 0.4f, 0.4f };
    CreateFloor(client, d2e::Vec2{ -4.5f,  2.5f }, smallFloorScale);
    CreateFloor(client, d2e::Vec2{ -4.5f, -2.5f }, smallFloorScale);
    CreateFloor(client, d2e::Vec2{  0.0f,  0.0f }, largeFloorScale);
    CreateFloor(client, d2e::Vec2{  4.5f, -2.5f }, smallFloorScale);
    CreateFloor(client, d2e::Vec2{  4.5f,  2.5f }, smallFloorScale);

    // Wall boundaries.
    CreateWall(client, d2e::Vec2{  0.0f,  4.5f }, d2e::Vec2{ 16.0f, 0.1f });
    CreateWall(client, d2e::Vec2{  0.0f, -4.5f }, d2e::Vec2{ 16.0f, 0.1f });
    CreateWall(client, d2e::Vec2{  8.0f,  0.0f }, d2e::Vec2{  0.1f, 9.0f });
    CreateWall(client, d2e::Vec2{ -8.0f,  0.0f }, d2e::Vec2{  0.1f, 9.0f });

    // Ping display.
    {
        d2e::WeakRef<d2e::GameObject> ping = scene->CreateGameObject();
        constexpr d2e::Ulid id = d2e::Engine::PING_DISPLAY_ULID;
        ping->SetId(id);

        d2e::WeakRef<d2e::PingDisplay> pingDisplay = ping->AddComponent<d2e::PingDisplay>();
        pingDisplay->SetSyncValuesOnUpdate(true);

        d2e::WeakRef<d2e::Transform> transform = ping->GetComponent<d2e::Transform>();
        transform->translation = d2e::Vec2{ -7.8f, 4.38f };

        if (uploadLevelData)
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

        if (uploadLevelData)
        {
            d2eNet::Packet packet;
            packet.AddLineWithId(id);
            packet.AddType<d2e::BattleTimer>(id, timer->Serialize());
            packet.AddSyncObject(id);
            client->AddPacketToSend(packet);
        }
    }

    if (!uploadLevelData)
    {
        d2e::WeakRef<GameScene> gameScene = scene.Cast<GameScene>();
        gameScene->GetPlayer().CreatePrefab(scene, false);
        gameScene->GetOtherPlayer().CreatePrefab(scene, true);
        GAME_LOG("Locally created the players");
    }

    client->ServerProcessedPacketsConfirmation([&] { LoadingCompleted(); });

    SplatAnimationManager::Instance()->InitPool(scene);
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
    d2e::WeakRef<d2e::GameObject> wall = mParent->CreateGameObject();
    const d2e::Ulid id = wall->GetId();

    d2e::WeakRef<d2e::Transform> transform = wall->GetComponent<d2e::Transform>();
    transform->translation = translation;

    d2e::WeakRef<d2e::StaticBoxCollider> bc = wall->AddComponent<d2e::StaticBoxCollider>();
    bc->SetHalfExtents(halfExtents);

    d2e::WeakRef<d2e::Tag> tag = wall->AddComponent<d2e::Tag>();
    tag->tag = d2e::ComponentTag::WALL;

    if (client->GetId() == 1)
    {
        d2eNet::Packet packet;
        packet.AddLineWithId(id);
        packet.AddType<d2e::Transform>(id, transform->Serialize());
        packet.AddType<d2e::StaticBoxCollider>(id, bc->Serialize());
        packet.AddType<d2e::Tag>(id, tag->Serialize());
        client->AddPacketToSend(packet);
    }
}

void LoadingLevelState::CreateFloor(d2e::WeakRef<d2eNet::Client> client, const d2e::Vec2 translation, const d2e::Vec2 scale)
{
    d2e::WeakRef<d2e::GameObject> floorObject = mParent->CreateGameObject();
    const d2e::Ulid id = floorObject->GetId();

    d2e::WeakRef<d2e::Sprite> floorSprite = floorObject->AddComponent<d2e::Sprite>("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Floor.png");

    d2e::WeakRef<d2e::Transform> transform = floorObject->GetComponent<d2e::Transform>();
    transform->translation = translation;
    transform->scale = scale;

    d2e::WeakRef<d2e::StaticBoxCollider> box = floorObject->AddComponent<d2e::StaticBoxCollider>();
    box->SetHalfExtents(floorSprite->GetHalfExtentsWorldSpace());

    if (client->GetId() == 1)
    {
        d2eNet::Packet packet;
        packet.AddLineWithId(id);
        packet.AddType<d2e::Sprite>(id, floorSprite->Serialize());
        packet.AddType<d2e::Transform>(id, transform->Serialize());
        packet.AddType<d2e::StaticBoxCollider>(id, box->Serialize());
        client->AddPacketToSend(packet);
    }
}

} // Namespace d2eGame.