#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "hold/HoldingData.h"

namespace UKControllerPlugin::Hold {

    extern const HoldingData holdSerializerInvalid;

    /*
        Create HoldingData from JSON
    */
    void from_json(const nlohmann::json& json, HoldingData& holdingData);

    void from_json_with_restrictions(
        const nlohmann::json& json,
        HoldingData& holdingData,
        const UKControllerPlugin::Bootstrap::PersistenceContainer& container);

    /*
        Returns true if the holding data is valid
    */
    auto JsonValid(const nlohmann::json& data) -> bool;

} // namespace UKControllerPlugin::Hold
