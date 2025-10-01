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

    mDeltaTime = mFrameClock.restart().asSeconds();
    const float delta = TARGET_FRAME_TIME - mDeltaTime;
    if (delta > 0)
    {
        std::this_thread::sleep_for(std::chrono::duration<float>(delta));
    }

#ifdef DEV_CONFIGURATION
    mWindow->setTitle(std::format("d2e - DEV - {} fps", 1.0f / (mDeltaTime + delta)));
#endif // DEV_CONFIGURATION.
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

} // Namespace d2e.
