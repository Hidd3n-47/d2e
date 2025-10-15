#pragma once

#include "d2e/ES/GameObject.h"

#include "d2e/ES/Components/Animation.h"
#include "d2e/ES/Components/CircleCollider.h"
#include "d2e/ES/Components/CircleSprite.h"
#include "d2e/ES/Components/Movement.h"
#include "d2e/ES/Components/RectangleSprite.h"
#include "d2e/ES/Components/RigidBody.h"
#include "d2e/ES/Components/StaticBoxCollider.h"
#include "d2e/ES/Components/Tag.h"
#include "d2e/ES/Components/Transform.h"
#include "d2e/ES/Components/UiButton.h"

namespace d2e
{

inline static auto& GetComponentMap()
{
    static std::unordered_map<std::string, std::function<WeakRef<IComponent>(d2e::WeakRef<d2e::GameObject>)>> map = []
    {
        std::unordered_map<std::string, std::function<WeakRef<IComponent>(d2e::WeakRef<d2e::GameObject>)>> m;
        m["Animation"]         = [](auto go) { return WeakRef{ go->AddComponent<d2e::Animation>()         }.Cast<IComponent>(); };
        m["CircleCollider"]    = [](auto go) { return WeakRef{ go->AddComponent<d2e::CircleCollider>()    }.Cast<IComponent>(); };
        m["CircleSprite"]      = [](auto go) { return WeakRef{ go->AddComponent<d2e::CircleSprite>()      }.Cast<IComponent>(); };
        m["Movement"]          = [](auto go) { return WeakRef{ go->AddComponent<d2e::Movement>()          }.Cast<IComponent>(); };
        m["RectangleSprite"]   = [](auto go) { return WeakRef{ go->AddComponent<d2e::RectangleSprite>()   }.Cast<IComponent>(); };
        m["RigidBody"]         = [](auto go) { return WeakRef{ go->AddComponent<d2e::RigidBody>()         }.Cast<IComponent>(); };
        m["StaticBoxCollider"] = [](auto go) { return WeakRef{ go->AddComponent<d2e::StaticBoxCollider>() }.Cast<IComponent>(); };
        m["Tag"]               = [](auto go) { return WeakRef{ go->AddComponent<d2e::Tag>()               }.Cast<IComponent>(); };
        m["UiButton"]          = [](auto go) { return WeakRef{ go->AddComponent<d2e::UiButton>()          }.Cast<IComponent>(); };
        m["Transform"]         = [](auto go) { return go->GetComponent<d2e::Transform>().Cast<IComponent>();                    };
        return m;
        }();
    return map;
}

} // Namespace d2e.