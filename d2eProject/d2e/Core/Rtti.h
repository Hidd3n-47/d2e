#pragma once

#include "d2e/ES/GameObject.h"

#define D2E_COMPONENT(T) inline T() \
    { std::string typeName = typeid(T).name(); \
      sAddComponentMap[typeName] = [](d2e::WeakRef<d2e::GameObject> gameObject) { gameObject->AddComponent<T>(); }; }

namespace d2e
{

inline static std::unordered_map<std::string, std::function<void(d2e::WeakRef<d2e::GameObject>)>> sAddComponentMap;
inline static std::unordered_map<std::string, std::function<void(d2e::WeakRef<d2e::GameObject>)>> sRemoveComponentMap;

} // Namespace d2e.