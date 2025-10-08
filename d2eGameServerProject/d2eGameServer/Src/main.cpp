#include "Core/ServerManager.h"

int main(int, char**)
{
    if (d2eServer::ServerManager::Instance()->Init())
    {
        d2eServer::ServerManager::Instance()->Run();
    }
    d2eServer::ServerManager::Instance()->Destroy();

    system("PAUSE");
}
