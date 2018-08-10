#include "time_manager.h"

namespace Ming3D
{
    void TimeManager::Initialise()
    {
        mStartClock = clock();
        UpdateTime();
    }

    void TimeManager::UpdateTime()
    {
        std::clock_t endClock = clock();
        double elapsedSeconds = double(endClock - mCurrentClock) / CLOCKS_PER_SEC;
        mDeltaTime = elapsedSeconds;
        mTime += mDeltaTime;
        mCurrentClock = endClock;
    }

    float TimeManager::GetTimeSeconds()
    {
        return mTime;
    }

    float TimeManager::GetDeltaTimeSeconds()
    {
        return mDeltaTime;
    }
}
