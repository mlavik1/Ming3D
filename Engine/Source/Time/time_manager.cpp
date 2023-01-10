#include "time_manager.h"

namespace Ming3D
{
    void TimeManager::Initialise()
    {
        mStartTimePoint = std::chrono::steady_clock::now();
        mCurrentTimePoint = mStartTimePoint;
        UpdateTime();
    }

    void TimeManager::UpdateTime()
    {


        auto newTimePoint = std::chrono::steady_clock::now();
        std::chrono::duration<double> duration = newTimePoint - mCurrentTimePoint;
        mDeltaTime = duration.count();
        mTime += mDeltaTime;
        mCurrentTimePoint = newTimePoint;
    }

    float TimeManager::GetTimeSeconds()
    {
        return static_cast<float>(mTime);
    }

    float TimeManager::GetDeltaTimeSeconds()
    {
        return static_cast<float>(mDeltaTime);
    }
}
