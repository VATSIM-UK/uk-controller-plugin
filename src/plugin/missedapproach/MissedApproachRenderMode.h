#pragma once

namespace UKControllerPlugin::MissedApproach {
    enum class MissedApproachRenderMode : unsigned int
    {
        None = 0,
        Line = 1,
        Circle = 2,
        Invalid = 4
    };

    inline MissedApproachRenderMode operator|(MissedApproachRenderMode first, MissedApproachRenderMode second)
    {
        return static_cast<MissedApproachRenderMode>(
            static_cast<unsigned int>(first) | static_cast<unsigned int>(second));
    }

    inline MissedApproachRenderMode operator&(MissedApproachRenderMode first, MissedApproachRenderMode second)
    {
        return static_cast<MissedApproachRenderMode>(
            static_cast<unsigned int>(first) & static_cast<unsigned int>(second));
    }
    
    inline bool operator==(MissedApproachRenderMode first, MissedApproachRenderMode second)
    {
        return static_cast<unsigned int>(first) == static_cast<unsigned int>(second);
    }
} // namespace UKControllerPlugin::MissedApproach
