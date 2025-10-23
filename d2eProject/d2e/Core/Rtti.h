#pragma once

#include "d2e/ES/GameObject.h"

#include "d2e/ES/Components/Animation.h"
#include "d2e/ES/Components/BattleTimer.h"
#include "d2e/ES/Components/CircleCollider.h"
#include "d2e/ES/Components/CircleSprite.h"
#include "d2e/ES/Components/Movement.h"
#include "d2e/ES/Components/PingDisplay.h"
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
    static std::unordered_map<std::string, std::function<WeakRef<IComponent>(WeakRef<GameObject>)>> map = []
    {
        std::unordered_map<std::string, std::function<WeakRef<IComponent>(WeakRef<GameObject>)>> m;
        m["Animation"]         = [](auto go) { return WeakRef{ go->AddComponent<Animation>()         }.Cast<IComponent>(); };
        m["BattleTimer"]       = [](auto go) { return WeakRef{ go->AddComponent<BattleTimer>()       }.Cast<IComponent>(); };
        m["CircleCollider"]    = [](auto go) { return WeakRef{ go->AddComponent<CircleCollider>()    }.Cast<IComponent>(); };
        m["CircleSprite"]      = [](auto go) { return WeakRef{ go->AddComponent<CircleSprite>()      }.Cast<IComponent>(); };
        m["Movement"]          = [](auto go) { return WeakRef{ go->AddComponent<Movement>()          }.Cast<IComponent>(); };
        m["PingDisplay"]       = [](auto go) { return WeakRef{ go->AddComponent<PingDisplay>()       }.Cast<IComponent>(); };
        m["RectangleSprite"]   = [](auto go) { return WeakRef{ go->AddComponent<RectangleSprite>()   }.Cast<IComponent>(); };
        m["RigidBody"]         = [](auto go) { return WeakRef{ go->AddComponent<RigidBody>()         }.Cast<IComponent>(); };
        m["StaticBoxCollider"] = [](auto go) { return WeakRef{ go->AddComponent<StaticBoxCollider>() }.Cast<IComponent>(); };
        m["Tag"]               = [](auto go) { return WeakRef{ go->AddComponent<Tag>()               }.Cast<IComponent>(); };
        m["UiButton"]          = [](auto go) { return WeakRef{ go->AddComponent<UiButton>()          }.Cast<IComponent>(); };
        m["Transform"]         = [](auto go) { return go->GetComponent<Transform>().Cast<IComponent>();                    };
        return m;
        }();
    return map;
}

} // Namespace d2e.