#pragma once
#include "pch/stdafx.h"
#include "hold/HoldingDataSerializer.h"
#include "hold/HoldingData.h"

namespace UKControllerPlugin {
    namespace Hold {

        const HoldingData holdSerializerInvalid = { 0, "INVALID" };

        /*
            Convert HoldingData to JSON
        */
        void to_json(nlohmann::json & json, const HoldingData & holdingData) {
            json = nlohmann::json { 
                {"id", holdingData.identifier},
                {"fix", holdingData.fix},
                {"description", holdingData.description},
                {"minimum_altitude", holdingData.minimum},
                {"maximum_altitude", holdingData.maximum},
                {"inbound_heading", holdingData.inbound},
                {"turn_direction", holdingData.turnDirection},
            };
        }

        /*
            Create HoldingData from JSON
        */
        void from_json(const nlohmann::json & json, HoldingData & holdingData) {
            if (!JsonValid(json)) {
                holdingData = holdSerializerInvalid;
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

        /*
            Returns true if the holding data is valid
        */
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
                (data.at("turn_direction") == "left" || data.at("turn_direction") == "right");
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
