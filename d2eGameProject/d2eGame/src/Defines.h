#pragma once

#ifdef DEV_CONFIGURATION
#define DEBUG_BREAK() __debugbreak()
#define DEBUG(...) __VA_ARGS__
#else  // DEV_CONFIGURATION.
#define DEBUG_BREAK()
#define DEBUG(...)
#endif // ! DEV_CONFIGURATION.

#ifdef GAME_DEBUG
#define GAME_LOG(...)   d2eGame::GameManager::Instance()->GetLog()->Debug(__VA_ARGS__)
#define GAME_WARN(...)  d2eGame::GameManager::Instance()->GetLog()->Warn(__VA_ARGS__)
#define GAME_ERROR(...) d2eGame::GameManager::Instance()->GetLog()->Error(__VA_ARGS__)
#else // GAME_DEBUG.
#define GAME_LOG(...)
#define GAME_WARN(...)
#define GAME_ERROR(...)
#endif // !GAME_DEBUG.