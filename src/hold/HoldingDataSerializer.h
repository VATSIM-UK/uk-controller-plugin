#pragma once
#include "pch/stdafx.h"
#include "hold/HoldingData.h"

namespace UKControllerPlugin {
    namespace Hold {

        extern const HoldingData holdSerializerInvalid;

        /*
            Convert HoldingData to JSON
        */
        void to_json(nlohmann::json & json, const HoldingData & holdingData);

        /*
            Create HoldingData from JSON
        */
        void from_json(const nlohmann::json & json, HoldingData & holdingData);

        /*
            Returns true if the holding data is valid
        */
        bool JsonValid(const nlohmann::json & data);

    }  // namespace Hold
}  // namespace UKControllerPlugin
