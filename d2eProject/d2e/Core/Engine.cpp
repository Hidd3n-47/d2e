#include "d2ePch.h"
#include "Engine.h"

#include <thread>

#include "ES/Scene.h"
#include "Input/InputManager.h"

namespace d2e
{

std::unique_ptr<Engine> Engine::mInstance = std::make_unique<Engine>();

void Engine::Init()
{
    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode({ static_cast<uint32_t>(mWindowSize.x), static_cast<uint32_t>(mWindowSize.y) }), "d2e");
    //mWindow->setFramerateLimit(TARGET_FRAMES);

    mInputManager = std::make_unique<InputManager>();
    DEBUG(mLog = std::make_unique<Log>("d2e Engine"));

    DEBUG_LOG("d2e engine initialized.");
}

void Engine::Run()
{
    while (mWindow->isOpen())
    {
        Update();
        Render();
    }
}

void Engine::Destroy()
{
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

void Engine::Update()
{
    const std::chrono::time_point frameStart = std::chrono::high_resolution_clock::now();

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
    }

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
    if (frameTime < std::chrono::duration<float>(1.0f / 120.0f))
    {
        const std::chrono::duration sleepTime = TARGET_FRAME_TIME - frameTime;
        std::this_thread::sleep_for(sleepTime);

        // Since sleep_for is not precise, it is possible it didn't sleep for enough time, therefore manually stall with a while loop.
        while (std::chrono::high_resolution_clock::now() - frameStart < TARGET_FRAME_TIME) {}
    }

#ifdef DEV_CONFIGURATION
    mWindow->setTitle(std::format("d2e - DEV - {} fps", 1.0f / (std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - frameStart).count())));
#endif // DEV_CONFIGURATION.
}

void Engine::Render() const
{
    static sf::Texture t{ "E:/Programming/d2e/d2eGameProject/d2eGame/Assets/s1.png" };
    static sf::Sprite s{ t };
    s.setColor(sf::Color{ 255, 0, 132, 255 });
    s.setPosition({ 1000.0f, 465.f });
    //s.setScale({ 0.5, 0.5 });

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
    mWindow->draw(s);
    mWindow->display();
}

} // Namespace d2e.
