#ifndef MING3D_TIMEMANAGER_H
#define MING3D_TIMEMANAGER_H

#include <ctime>

namespace Ming3D
{
    class TimeManager
    {
    private:
        std::clock_t mStartClock;
        std::clock_t mCurrentClock;
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
