#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace InitialAltitude {

        typedef struct InitialAltitudeData {
            const std::string identifier;
            const int altitude;

            bool operator< (const InitialAltitudeData & compare)
            {
                return identifier < compare.identifier;
            }
        } InitialAltitudeData;

    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
