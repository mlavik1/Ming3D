#ifndef MING3D_TIMEMANAGER_H
#define MING3D_TIMEMANAGER_H

#include <chrono>

namespace Ming3D
{
    class TimeManager
    {
    private:
        std::chrono::time_point<std::chrono::steady_clock>  mStartTimePoint;
        std::chrono::time_point<std::chrono::steady_clock>  mCurrentTimePoint;
        double mTime = 0.0f;
        double mDeltaTime = 0.0f;

    public:
        void Initialise();
        void UpdateTime();
        float GetTimeSeconds();
        float GetDeltaTimeSeconds();
    };
}

#endif
