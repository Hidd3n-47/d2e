#include "d2ePch.h"

#include <core/Engine.h>

int main(int, char**)
{
    d2e::Engine::Instance()->Init();
    d2e::Engine::Instance()->Run();
    d2e::Engine::Instance()->Destroy();

    return 0;

}