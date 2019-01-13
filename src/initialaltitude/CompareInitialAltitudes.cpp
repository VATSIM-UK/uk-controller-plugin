#include "pch/stdafx.h"
#include "initialaltitude/CompareInitialAltitudes.h"
#include "initialaltitude/InitialAltitudeData.h"

using UKControllerPlugin::InitialAltitude::InitialAltitudeData;

namespace UKControllerPlugin {
    namespace InitialAltitude {
        bool CompareInitialAltitudes::operator()(const InitialAltitudeData & initial, std::string id) const
        {
            return initial.identifier < id;
        }
        bool CompareInitialAltitudes::operator()(std::string id, const InitialAltitudeData & initial) const
        {
            return id < initial.identifier;
        }
        bool CompareInitialAltitudes::operator()(const InitialAltitudeData & a, const InitialAltitudeData & b) const
        {
            return a.identifier < b.identifier;
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
