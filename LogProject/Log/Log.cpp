#include "Log.h"

namespace d2e
{

Log::Log(std::string&& logTitle, const bool displayTimestamp /* = true */)
    : mLogTitle(std::move(logTitle))
    , mDisplayTimestamp(displayTimestamp)
{
    // Empty.
}

} // Namespace d2e.