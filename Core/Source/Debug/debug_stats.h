#ifndef MING3D_DEBUGSTATS_H
#define MING3D_DEBUGSTATS_H

#include <unordered_map>
#include <string>
#include "Debug/debug.h"

namespace Ming3D
{
    template<typename T>
    class DebugStatController
    {
    private:
        std::unordered_map<std::string, T> mStats;

    public:
        void SetValue(std::string statName, T value)
        {
            mStats[statName] = value;
        }

        T GetValue(std::string statName)
        {
            return mStats[statName];
        }

        void Clear()
        {
            mStats.clear();
        }
        // TODO: iterator
        std::unordered_map<std::string, T> GetStats()
        {
            return mStats;
        }
    };

    extern DebugStatController<int>* GStatControllerInt;
    extern DebugStatController<float>* GStatControllerFloat;
    extern DebugStatController<int>* GFrameStatControllerInt;
    extern DebugStatController<float>* GFrameStatControllerFloat;

    inline void ClearFrameStats()
    {
        GFrameStatControllerInt->Clear();
        GFrameStatControllerFloat->Clear();
    }

    inline void DebugStats_PrintAllStats()
    {
        LOG_INFO() << "---------- DEBUG STATS ----------";
        for (auto stat : GStatControllerInt->GetStats())
        {
            LOG_INFO() << stat.first << ": " << stat.second;
        }
        for (auto stat : GStatControllerFloat->GetStats())
        {
            LOG_INFO() << stat.first << ": " << stat.second;
        }
        LOG_INFO() << "---------- FRAME STATS ----------";
        for (auto stat : GFrameStatControllerInt->GetStats())
        {
            LOG_INFO() << stat.first << ": " << stat.second;
        }
        for (auto stat : GFrameStatControllerFloat->GetStats())
        {
            LOG_INFO() << stat.first << ": " << stat.second;
        }
        LOG_INFO() << "--------------------";
    }
}

//#define DEBUG_STATS_ENABLED

// --- INT STATS ---
#define GET_DEBUG_STAT_INT(StatName) GStatControllerInt->GetValue(StatName)
#define GET_FRAME_STAT_INT(StatName) GFrameStatControllerInt->GetValue(StatName)

#ifdef MING3D_DEBUG_STATS_ENABLED
#define SET_DEBUG_STAT_INT(StatName, Value) \
GStatControllerInt->SetValue(StatName, Value);
#define SET_FRAME_STAT_INT(StatName, Value) \
GFrameStatControllerInt->SetValue(StatName, Value);
#else
#define SET_DEBUG_STAT_INT(StatName, Value)
#define SET_FRAME_STAT_INT(StatName, Value)

#endif

#define ADD_DEBUG_STAT_INT(StatName, Value) SET_DEBUG_STAT_INT(StatName, GET_DEBUG_STAT_INT(StatName) + Value)
#define ADD_FRAME_STAT_INT(StatName, Value) SET_FRAME_STAT_INT(StatName, GET_FRAME_STAT_INT(StatName) + Value)

// --- FLOAT STATS ---
#define GET_DEBUG_STAT_FLOAT(StatName) GStatControllerFloat->GetValue(StatName)
#define GET_FRAME_STAT_FLOAT(StatName) GFrameControllerFloat->GetValue(StatName)

#ifdef MING3D_DEBUG_STATS_ENABLED
#define SET_DEBUG_STAT_FLOAT(StatName, Value) \
GStatControllerFloat->SetValue(StatName, Value);
#define SET_FRAME_STAT_FLOAT(StatName, Value) \
GFrameControllerFloat->SetValue(StatName, Value);
#else
#define SET_DEBUG_STAT_FLOAT(StatName, Value)
#define SET_FRAME_STAT_FLOAT(StatName, Value)
#endif

#define ADD_DEBUG_STAT_FLOAT(StatName, Value) SET_DEBUG_STAT_FLOAT(StatName, GET_DEBUG_STAT_FLOAT(StatName) + Value)
#define ADD_FRAME_STAT_FLOAT(StatName, Value) SET_DEBUG_STAT_FLOAT(StatName, GET_DEBUG_STAT_FLOAT(StatName) + Value)

#endif
