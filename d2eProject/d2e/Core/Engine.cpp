#include "d2ePch.h"
#include "Engine.h"

#include <d2eNet/Core/d2eNet.h>
#include <d2eNet/Core/Client.h>

#include "Input/InputManager.h"

#include "SerializationUtils.h"

#include "ES/Scene.h"
#include "ES/Components/Transform.h"

namespace d2e
{

std::unique_ptr<Engine> Engine::mInstance = std::make_unique<Engine>();

void Engine::Init()
{
    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode({ static_cast<uint32_t>(DEFAULT_SCREEN_SIZE.x), static_cast<uint32_t>(DEFAULT_SCREEN_SIZE.y) }), "d2e");

    mInputManager = std::make_unique<InputManager>();
    DEBUG(mLog = std::make_unique<Log>("d2e Engine"));

    OrthoCameraValues cameraValues
    {
        .left   = -8.0f,
        .right  =  8.0f,
        .top    =  4.5f,
        .bottom = -4.5f,
        .screenWidth  = DEFAULT_SCREEN_SIZE.x,
        .screenHeight = DEFAULT_SCREEN_SIZE.y
    };
    mOrthoCamera = OrthoCamera{ cameraValues };

    d2eNet::d2eNet::Init();

    DEBUG_LOG("d2e engine initialized.");
}

void Engine::Run()
{
    while (mWindow->isOpen() && mRunning)
    {
        StartFrame();
        Input();

        if (mActiveScene && !mActiveScene->IsScenePaused())
        {
            SceneUpdate();
        }

        if (mActiveScene && !mActiveScene->IsScenePaused())
        {
            Update();
        }

        PostUpdate();
        Render();
        ProcessNetworkPackets();
        EndFrame();

//#ifdef DEV_CONFIGURATION
        mWindow->setTitle(std::format("d2e - DEV - {} fps", 1.0f / (std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - mFrameStart).count())));
//#endif // DEV_CONFIGURATION.
    }
}

void Engine::Destroy()
{
    d2eNet::d2eNet::Destroy();

    DEBUG_LOG("d2e engine destroyed.");
}

void Engine::ConnectClientToServer(const int ip1, const int ip2, const int ip3, const int ip4, const uint16_t port)
{
    mClient = std::make_unique<d2eNet::Client>();
    if (!mClient->Init(static_cast<uint8_t>(ip1), static_cast<uint8_t>(ip2), static_cast<uint8_t>(ip3), static_cast<uint8_t>(ip4), port))
    {
        DEBUG_ERROR("Failed to connect Host to Client.");
    }
}

void Engine::StartFrame()
{
    mFrameStart = std::chrono::high_resolution_clock::now();
}

void Engine::EndFrame() const
{
    const std::chrono::duration frameTime = std::chrono::high_resolution_clock::now() - mFrameStart;
    if (frameTime < std::chrono::duration<float>(TARGET_FRAME_TIME))
    {
        const std::chrono::duration sleepTime = TARGET_FRAME_TIME - frameTime;
        std::this_thread::sleep_for(sleepTime);

        // Since sleep_for is not precise, it is possible it didn't sleep for enough time, therefore manually stall with a while loop.
        while (std::chrono::high_resolution_clock::now() - mFrameStart < TARGET_FRAME_TIME) {}
    }
}

void Engine::Input() const
{
    mInputManager->StartFrame();

    while (const std::optional event = mWindow->pollEvent())
    {
        if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
        {
            mWindow->close();
        }

        if (const sf::Event::Resized* resized = event->getIf<sf::Event::Resized>())
        {
            mWindow->setSize(resized->size);
        }

        if (const sf::Event::KeyPressed* key = event->getIf<sf::Event::KeyPressed>())
        {
            mInputManager->KeyDown(key->code);
        }

        if (const sf::Event::KeyReleased* key = event->getIf<sf::Event::KeyReleased>())
        {
            mInputManager->KeyUp(key->code);
        }

        if (const sf::Event::MouseButtonPressed* button = event->getIf<sf::Event::MouseButtonPressed>())
        {
            mInputManager->MouseDown(button->button);
        }

        if (const sf::Event::MouseButtonReleased* button = event->getIf<sf::Event::MouseButtonReleased>())
        {
            mInputManager->MouseUp(button->button);
        }

        if (const sf::Event::MouseMoved* moved = event->getIf<sf::Event::MouseMoved>())
        {
            mInputManager->SetMousePosition(Vec2{ static_cast<float>(moved->position.x), static_cast<float>(moved->position.y) });
        }
    }
}

void Engine::SceneUpdate() const
{
    if (mActiveScene != nullptr)
    {
        mActiveScene->SceneUpdate();
    }
}

void Engine::Update() const
{
    if (mActiveScene)
    {
        mActiveScene->Update(mDeltaTime);
    }
}

void Engine::PostUpdate()
{
    if (mSceneToChangeTo)
    {
        //delete mActiveScene;

        mActiveScene = mSceneToChangeTo;
        mActiveScene->InitScene();

        mSceneToChangeTo = nullptr;
    }
}


void Engine::Render() const
{
    mWindow->clear();

    if (mActiveScene)
    {
        mActiveScene->Render(WeakRef{ mWindow.get() }, mOrthoCamera);
    }
    else
    {
        DEBUG_WARN("No active scene set to render.");
        DEBUG_BREAK();
    }

    mWindow->display();
}

void Engine::ProcessNetworkPackets() const
{
    if (!mClient)
    {
        return;
    }

    std::optional<d2eNet::Packet> packet = mClient->GetPacketReceived();
    while (packet)
    {
        for (d2eNet::Packet::Iterator it = packet->Begin(); it != packet->End(); ++it)
        {
            const std::string packetString = it.GetPacketLineString();

            switch (it.GetPacketLineType())
            {
            case d2eNet::PacketLineType::UPDATE_COMPONENT:
            {
                const size_t firstDelimiter  = packetString.find(SerializeUtils::DELIMITER);
                const size_t secondDelimiter = packetString.find(SerializeUtils::DELIMITER, firstDelimiter + 1);

                const uint64_t id = std::stoull(packetString.substr(0, firstDelimiter));
                const std::string componentName  = packetString.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1);
                const std::string componentValue = packetString.substr(secondDelimiter + 1);

                mActiveScene->GetGameObject(Ulid{ id })->GetComponent(componentName)->Deserialize(componentValue);
                break;
            }
            case d2eNet::PacketLineType::PLAYER_TWO_JOINED:
            {
                if (mOnPlayerTwoJoined)
                {
                    mOnPlayerTwoJoined();
                }
                break;
            }
            case d2eNet::PacketLineType::SERVER_HANDLED_PACKET_CONFIRM:
            {
                mClient->ServerHandledPacketQuery(std::stoul(packetString));
                break;
            }
            case d2eNet::PacketLineType::PLAYER_DISCONNECTED:
                if (mOnPlayerDisconnected)
                {
                    mOnPlayerDisconnected();
                }
                break;
            case d2eNet::PacketLineType::ADD_COMPONENT:
            case d2eNet::PacketLineType::ADD_GAME_OBJECT:
            case d2eNet::PacketLineType::SYNC_GAME_OBJECT_ACROSS_NETWORK:
            default:
                DEBUG_WARN("Received packet that is not processed: <{}>", packetString);
                break;
            }
        }

        packet = mClient->GetPacketReceived();
    }
}

} // Namespace d2e.
