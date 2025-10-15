#include "GameScene.h"

#include <d2e/ES/Scene.h>
#include <d2e/core/Engine.h>

#include <d2e/ES/Components/Tag.h>
#include <d2e/ES/Components/Transform.h>
#include <d2e/ES/Components/RectangleSprite.h>
#include <d2e/ES/Components/StaticBoxCollider.h>

#include "d2eNet/Core/Client.h"
#include "d2eNet/Core/Packet.h"

namespace d2eGame
{

GameScene::GameScene()
{
    mScene = d2e::Engine::Instance()->CreateScene();
}

void GameScene::InitGameScene()
{
    const d2e::Vec2 windowSize = d2e::Engine::Instance()->GetWindowSize();

    auto client = d2e::Engine::Instance()->GetClient();

    {
        d2eNet::Packet packet;

        d2e::WeakRef<d2e::GameObject> floorObject = mScene->CreateGameObject();
        const uint32_t id = floorObject->GetId();
        packet.AddLineWithId(id);

        d2e::WeakRef<d2e::RectangleSprite> floorSprite = floorObject->AddComponent<d2e::RectangleSprite>();
        floorSprite->SetHalfExtents(windowSize * d2e::Vec2{ 0.2f, 0.05f });
        floorSprite->SetColor(sf::Color::Blue);
        packet.AddType<d2e::RectangleSprite>(id, floorSprite->Serialize());

        auto transform = floorObject->GetComponent<d2e::Transform>();
        transform->translation = windowSize * d2e::Vec2{ 0.5f, 0.5f };
        packet.AddType<d2e::Transform>(id, transform->Serialize());

        d2e::WeakRef<d2e::StaticBoxCollider> bc = floorObject->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(floorSprite->GetHalfExtents());
        packet.AddType<d2e::StaticBoxCollider>(id, bc->Serialize());

        client->AddPacketToSend(packet);
    }

    // Wall boundaries.
    {
        d2eNet::Packet packet;

        d2e::WeakRef<d2e::GameObject> leftWall = mScene->CreateGameObject();
        const uint32_t id = leftWall->GetId();
        packet.AddLineWithId(id);

        auto transform = leftWall->GetComponent<d2e::Transform>();
        transform->translation = windowSize * d2e::Vec2{ 0.0f, 0.5f };
        packet.AddType<d2e::Transform>(id, transform->Serialize());

        auto bc = leftWall->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(d2e::Vec2{5.0f, windowSize.y });
        packet.AddType<d2e::StaticBoxCollider>(id, bc->Serialize());

        auto tag = leftWall->AddComponent<d2e::Tag>();
        tag->tag = d2e::ComponentTag::WALL;
        packet.AddType<d2e::Tag>(id, tag->Serialize());

        client->AddPacketToSend(packet);
    }

    {
        d2eNet::Packet packet;

        d2e::WeakRef<d2e::GameObject> rightWall = mScene->CreateGameObject();
        const uint32_t id = rightWall->GetId();
        packet.AddLineWithId(id);

        auto transform = rightWall->GetComponent<d2e::Transform>();
        transform->translation = windowSize * d2e::Vec2{ 1.0f, 0.5f };
        packet.AddType<d2e::Transform>(id, transform->Serialize());

        auto bc = rightWall->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(d2e::Vec2{ 5.0f, windowSize.y });
        packet.AddType<d2e::StaticBoxCollider>(id, bc->Serialize());

        auto tag = rightWall->AddComponent<d2e::Tag>();
        tag->tag = d2e::ComponentTag::WALL;
        packet.AddType<d2e::Tag>(id, tag->Serialize());

        client->AddPacketToSend(packet);
    }

    {
        d2eNet::Packet packet;

        d2e::WeakRef<d2e::GameObject> topWall = mScene->CreateGameObject();
        const uint32_t id = topWall->GetId();
        packet.AddLineWithId(id);

        auto transform = topWall->GetComponent<d2e::Transform>();
        transform->translation = windowSize * d2e::Vec2{ 0.5f, 0.0f };
        packet.AddType<d2e::Transform>(id, transform->Serialize());

        auto bc = topWall->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(d2e::Vec2{ windowSize.x, 5.0f });
        packet.AddType<d2e::StaticBoxCollider>(id, bc->Serialize());

        auto tag = topWall->AddComponent<d2e::Tag>();
        tag->tag = d2e::ComponentTag::WALL;
        packet.AddType<d2e::StaticBoxCollider>(id, bc->Serialize());

        client->AddPacketToSend(packet);
    }

    //todo can extract this into a function.
    {
        d2eNet::Packet packet;

        d2e::WeakRef<d2e::GameObject> bottomWall = mScene->CreateGameObject();
        const uint32_t id = bottomWall->GetId();
        packet.AddLineWithId(id);

        auto transform = bottomWall->GetComponent<d2e::Transform>();
        transform->translation = windowSize * d2e::Vec2{ 0.5f, 1.0f };
        packet.AddType<d2e::Transform>(id, transform->Serialize());

        auto bc = bottomWall->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(d2e::Vec2{ windowSize.x, 5.0f });
        packet.AddType<d2e::StaticBoxCollider>(id, bc->Serialize());

        auto tag = bottomWall->AddComponent<d2e::Tag>();
        tag->tag = d2e::ComponentTag::WALL;
        packet.AddType<d2e::Tag>(id, tag->Serialize());

        client->AddPacketToSend(packet);
    }

    mPlayer.CreatePrefab(mScene);
}
} // Namespace d2eGame.