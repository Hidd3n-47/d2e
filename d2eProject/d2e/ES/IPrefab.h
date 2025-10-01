#pragma once

#include <d2e/ES/Scene.h>

namespace d2e
{

class IPrefab
{
public:
    virtual ~IPrefab() = default;

    virtual void CreatePrefab(WeakRef<Scene> scene) = 0;
};

} // Namespace d2e.