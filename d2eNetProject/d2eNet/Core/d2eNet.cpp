#include "d2eNet.h"

#include <enet/enet.h>

namespace d2eNet
{

bool d2eNet::Init()
{
    return enet_initialize() == 0;
}

void d2eNet::Destroy()
{
    enet_deinitialize();
}

} // Namespace d2eNet.