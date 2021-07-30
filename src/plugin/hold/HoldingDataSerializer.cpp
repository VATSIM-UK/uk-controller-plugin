#pragma once
#include "pch/stdafx.h"
#include "hold/HoldingDataSerializer.h"
#include "hold/HoldingData.h"
#include "hold/HoldRestrictionSerializer.h"
#include "hold/DeemedSeparatedHoldSerializer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Hold {

        const HoldingData holdSerializerInvalid = { 0, "INVALID" };

        /*
            Create HoldingData from JSON
        */
        void from_json(
            const nlohmann::json & json,
            HoldingData & holdingData
        ) {
            if (!JsonValid(json)) {
                holdingData = { 0, "INVALID" };
                return;
            }

            json.at("id").get_to(holdingData.identifier);
            json.at("fix").get_to(holdingData.fix);
            json.at("description").get_to(holdingData.description);
            json.at("minimum_altitude").get_to(holdingData.minimum);
            json.at("maximum_altitude").get_to(holdingData.maximum);
            json.at("inbound_heading").get_to(holdingData.inbound);
            json.at("turn_direction").get_to(holdingData.turnDirection);
        }

        void from_json_with_restrictions(
            const nlohmann::json & json,
            HoldingData & holdingData,
            const PersistenceContainer & container
        ) {
            from_json(json, holdingData);

            if (holdingData == holdSerializerInvalid) {
                return;
            }

            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> restrictions;
            hold_restriction_from_json(json.at("restrictions"), std::move(restrictions), container);
            holdingData.restrictions = std::move(restrictions);

            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparatedHolds;
            deemed_separated_from_json(json.at("deemed_separated_holds"), deemedSeparatedHolds);
            holdingData.deemedSeparatedHolds = std::move(deemedSeparatedHolds);
        }

        bool JsonValid(const nlohmann::json & data)
        {
            return data.is_object() &&
                data.find("id") != data.end() &&
                data.at("id").is_number_integer() &&
                data.find("fix") != data.end() &&
                data.at("fix").is_string() &&
                data.find("description") != data.end() &&
                data.at("description").is_string() &&
                data.find("minimum_altitude") != data.end() &&
                data.at("minimum_altitude").is_number_integer() &&
                data.find("maximum_altitude") != data.end() &&
                data.at("maximum_altitude").is_number_integer() &&
                data.find("inbound_heading") != data.end() &&
                data.at("inbound_heading").is_number_integer() &&
                data.find("turn_direction") != data.end() &&
                data.at("turn_direction").is_string() &&
                (data.at("turn_direction") == "left" || data.at("turn_direction") == "right") &&
                data.find("restrictions") != data.end() &&
                data.at("restrictions").is_array() &&
                data.contains("deemed_separated_holds") &&
                data.at("deemed_separated_holds").is_array();
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
