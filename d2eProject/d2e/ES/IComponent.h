#pragma once

namespace d2e
{

class IComponent
{
public:
    virtual ~IComponent() = default;

    virtual void OnComponentAdded()     = 0;
    virtual void OnComponentRemoved()   = 0;

    virtual void Update()               = 0;
};

}