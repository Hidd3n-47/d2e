#pragma once

#ifdef DEV_CONFIGURATION
#define DEBUG_BREAK() __debugbreak()
#define DEBUG(...) __VA_ARGS__
#else  // DEV_CONFIGURATION.
#define DEBUG_BREAK()
#define DEBUG(...)
#endif // ! DEV_CONFIGURATION.

#ifdef ENGINE_DEBUG
#define DEBUG_LOG(...)   d2e::Engine::Instance()->GetLog()->Debug(__VA_ARGS__)
#define DEBUG_WARN(...)  d2e::Engine::Instance()->GetLog()->Warn(__VA_ARGS__)
#define DEBUG_ERROR(...) d2e::Engine::Instance()->GetLog()->Error(__VA_ARGS__)
#else // ENGINE_DEBUG.
#define DEBUG_LOG(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif // !ENGINE_DEBUG.