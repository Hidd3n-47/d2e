#include "d2ePch.h"
#include "Engine.h"

#include <sfml/Graphics.hpp>

#include "ES/Scene.h"
#include "Input/InputManager.h"

namespace d2e
{

std::unique_ptr<Engine> Engine::mInstance = std::make_unique<Engine>();

void Engine::Init()
{
    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode({ 1920, 1080 }), "d2e");

    mInputManager = std::make_unique<InputManager>();

    Log::Debug("d2e engine initialized.");
}

void Engine::Run() const
{
    while (mWindow->isOpen())
    {
        Update();
        Render();
    }
}

void Engine::Destroy()
{
    Log::Debug("d2e engine destroyed.");
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

void Engine::Update() const
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
    }

    if (mActiveScene)
    {
        mActiveScene->Update();
    }
    else
    {
        Log::Warn("No active scene set to update.");
    }
}

void Engine::Render() const
{
    mWindow->clear();

    sf::RectangleShape s{ {500.0f, 500.0f} };
    s.setFillColor(sf::Color::Blue);
    sf::CircleShape circle{ 50.0f };
    circle.setFillColor(sf::Color::Green);

    mWindow->draw(s);
    mWindow->draw(circle);

    mWindow->display();
}

} // Namespace d2e.
