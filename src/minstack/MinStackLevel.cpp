#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace MinStack {

        /*
            Represents a minimum stack level
        */
        typedef struct MinStackLevel
        {
            // The MSL
            unsigned int msl;

            // When it was last updated
            std::chrono::system_clock::time_point updatedAt;

            // The time at which it was acknowledged
            std::chrono::system_clock::time_point acknowledgedAt;

        } MinStackLevel;
    }  // namespace MinStack
}  // namespace UKControllerPlugin