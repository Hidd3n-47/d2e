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
        d2e::WeakRef<d2e::GameObject>       floorObject = mScene->CreateGameObject();
        d2e::WeakRef<d2e::RectangleSprite>  floorSprite = floorObject->AddComponent<d2e::RectangleSprite>();

        floorSprite->SetHalfExtents(windowSize * d2e::Vec2{ 0.2f, 0.05f });
        floorSprite->SetColor(sf::Color::Blue);
        floorObject->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.5f, 0.5f };
        auto bc = floorObject->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(floorSprite->GetHalfExtents());

        d2eNet::Packet packet;
        packet.AddLineType(d2eNet::PacketLineType::ADD_GAME_OBJECT);
        packet.AddType<d2e::RectangleSprite>();
        client->SendPacket(packet.GetPacketString().c_str());
    }

    // Wall boundaries.
    {
        d2e::WeakRef<d2e::GameObject> leftWall = mScene->CreateGameObject();

        leftWall->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.0f, 0.5f };
        auto bc = leftWall->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(d2e::Vec2{5.0f, windowSize.y });

        auto tag = leftWall->AddComponent<d2e::Tag>();
        tag->tag = d2e::ComponentTag::WALL;

        d2eNet::Packet packet;
        packet.AddLineType(d2eNet::PacketLineType::ADD_GAME_OBJECT);
        packet.AddType<d2e::StaticBoxCollider>();
    }

    {
        d2e::WeakRef<d2e::GameObject> rightWall = mScene->CreateGameObject();

        rightWall->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 1.0f, 0.5f };
        auto bc = rightWall->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(d2e::Vec2{ 5.0f, windowSize.y });

        auto tag = rightWall->AddComponent<d2e::Tag>();
        tag->tag = d2e::ComponentTag::WALL;

        d2eNet::Packet packet;
        packet.AddLineType(d2eNet::PacketLineType::ADD_GAME_OBJECT);
        packet.AddType<d2e::StaticBoxCollider>();
    }

    {
        d2e::WeakRef<d2e::GameObject> topWall = mScene->CreateGameObject();

        topWall->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.5f, 0.0f };
        auto bc = topWall->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(d2e::Vec2{ windowSize.x, 5.0f });

        auto tag = topWall->AddComponent<d2e::Tag>();
        tag->tag = d2e::ComponentTag::WALL;

        d2eNet::Packet packet;
        packet.AddLineType(d2eNet::PacketLineType::ADD_GAME_OBJECT);
        packet.AddType<d2e::StaticBoxCollider>();
    }

    {
        d2e::WeakRef<d2e::GameObject> bottomWall = mScene->CreateGameObject();

        bottomWall->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.5f, 1.0f };
        auto bc = bottomWall->AddComponent<d2e::StaticBoxCollider>();
        bc->SetHalfExtents(d2e::Vec2{ windowSize.x, 5.0f });

        auto tag = bottomWall->AddComponent<d2e::Tag>();
        tag->tag = d2e::ComponentTag::WALL;

        d2eNet::Packet packet;
        packet.AddLineType(d2eNet::PacketLineType::ADD_GAME_OBJECT);
        packet.AddType<d2e::StaticBoxCollider>();
    }

    mPlayer.CreatePrefab(mScene);
}
} // Namespace d2eGame.