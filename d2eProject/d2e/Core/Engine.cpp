#include "d2ePch.h"
#include "Engine.h"

#include <thread>

#include <d2eNet/Core/d2eNet.h>
#include <d2eNet/Core/Client.h>

#include "ES/Scene.h"
#include "Input/InputManager.h"

namespace d2e
{

std::unique_ptr<Engine> Engine::mInstance = std::make_unique<Engine>();

void Engine::Init()
{
    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode({ static_cast<uint32_t>(mWindowSize.x), static_cast<uint32_t>(mWindowSize.y) }), "d2e");

    mInputManager = std::make_unique<InputManager>();
    DEBUG(mLog = std::make_unique<Log>("d2e Engine"));

    d2eNet::d2eNet::Init();

    DEBUG_LOG("d2e engine initialized.");
}

void Engine::Run()
{
    while (mWindow->isOpen() && mRunning)
    {
        Input();
        SendPackets();
        Update();
        Render();
        ReceivePackets();
    }
}

void Engine::Destroy()
{
    d2eNet::d2eNet::Destroy();

    DEBUG_LOG("d2e engine destroyed.");
}

WeakRef<Scene> Engine::CreateScene()
{
    mScenes.emplace_back(new Scene());
    return WeakRef{ mScenes.back() };
}

bool Engine::SetActiveScene(const WeakRef<Scene>& scene)
{
    if (scene.IsRefValid())
    {
        mActiveScene = scene.GetRawPtr();
        return true;
    }

    return false;
}

void Engine::ConnectClientToServer(const int ip1, const int ip2, const int ip3, const int ip4, const uint16_t port)
{
    mClient = std::make_unique<d2eNet::Client>();
    if (!mClient->Init(static_cast<uint8_t>(ip1), static_cast<uint8_t>(ip2), static_cast<uint8_t>(ip3), static_cast<uint8_t>(ip4), port))
    {
        DEBUG_ERROR("Failed to connect Host to Client.");
    }
}

void Engine::Input()
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
            mWindowSize = Vec2{ static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) };
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

void Engine::SendPackets() const
{
    if (!mClient)
    {
        return;
    }

    mClient->Update(3);
}

void Engine::Update() const
{
    const std::chrono::time_point frameStart = std::chrono::high_resolution_clock::now();

    if (mActiveScene)
    {
        mActiveScene->Update(mDeltaTime);
    }
    else
    {
        DEBUG_WARN("No active scene set to update.");
        DEBUG_BREAK();
    }

    const std::chrono::duration frameTime = std::chrono::high_resolution_clock::now() - frameStart;
    if (frameTime < std::chrono::duration<float>(TARGET_FRAME_TIME))
    {
        const std::chrono::duration sleepTime = TARGET_FRAME_TIME - frameTime;
        std::this_thread::sleep_for(sleepTime);

        // Since sleep_for is not precise, it is possible it didn't sleep for enough time, therefore manually stall with a while loop.
        while (std::chrono::high_resolution_clock::now() - frameStart < TARGET_FRAME_TIME) {}
    }

//todo uncomment when we get to final build, just want it for stats.
//#ifdef DEV_CONFIGURATION
    mWindow->setTitle(std::format("d2e - DEV - {} fps", 1.0f / (std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - frameStart).count())));
//#endif // DEV_CONFIGURATION.
}

void Engine::Render() const
{
    mWindow->clear();

    if (mActiveScene)
    {
        mActiveScene->Render(WeakRef{ mWindow.get() });
    }
    else
    {
        DEBUG_WARN("No active scene set to render.");
        DEBUG_BREAK();
    }

    mWindow->display();
}

void Engine::ReceivePackets()
{
    if (!mClient)
    {
        return;
    }

    mRunning = mRunning;
}

} // Namespace d2e.
