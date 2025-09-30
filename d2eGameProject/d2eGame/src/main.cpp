#include <d2e/src/d2ePch.h>

#include "Core/GameManager.h"

int main(int, char**)
{
    d2e::Engine::Instance()->Init();

    d2eGame::GameManager::Instance()->Init();

    d2e::Engine::Instance()->Run();
    d2e::Engine::Instance()->Destroy();

    return 0;

}