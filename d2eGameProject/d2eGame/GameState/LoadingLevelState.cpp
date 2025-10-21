#include "LoadingLevelState.h"

namespace d2eGame
{

class IGameState
{
public:
    virtual ~IGameState() = default;
    virtual void Update() = 0;
};

} // Namespace d2eGame.