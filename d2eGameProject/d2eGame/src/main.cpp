#include <d2e/src/d2ePch.h>
#include <d2e/core/Engine.h>

int main(int, char**)
{
    d2e::Engine::Instance()->Init();

    auto scene = d2e::Engine::Instance()->CreateScene();
    d2e::Engine::Instance()->SetActiveScene(scene);

    d2e::Engine::Instance()->Run();
    d2e::Engine::Instance()->Destroy();

    return 0;

}