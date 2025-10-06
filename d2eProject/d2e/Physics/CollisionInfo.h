#pragma once

namespace d2e
{

class GameObject;

struct CollisionInfo
{
    WeakRef<GameObject> instance;
    WeakRef<GameObject> other;
    Vec2 collisionPosition;
};

}