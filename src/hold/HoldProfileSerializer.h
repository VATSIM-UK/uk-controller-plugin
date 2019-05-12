#pragma once
#include "pch/stdafx.h"
#include "hold/HoldProfile.h"

namespace UKControllerPlugin {
    namespace Hold {

        extern const HoldProfile holdProfileSerializerInvalid;

        /*
            Convert HoldingData to JSON
        */
        void to_json(nlohmann::json & json, const HoldProfile & holdingData);

        /*
            Create HoldingData from JSON
        */
        void from_json(const nlohmann::json & json, HoldProfile & holdingData);

        /*
            Returns true if the holding data is valid
        */
        bool ProfileJsonValid(const nlohmann::json & data);

        /*
            Returns true if all the hold ids are integers
        */
        bool HoldIdsValid(const nlohmann::json & data);

    }  // namespace Hold
}  // namespace UKControllerPlugin
