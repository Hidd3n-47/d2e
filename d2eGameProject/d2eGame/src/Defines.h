#pragma once

#ifdef DEV_CONFIGURATION
#define DEBUG_BREAK() __debugbreak()
#else  // DEV_CONFIGURATION.
#define DEBUG_BREAK()
#endif // ! DEV_CONFIGURATION.