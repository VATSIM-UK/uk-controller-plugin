#include "pch/stdafx.h"
#include "hold/BuildHoldingData.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Convert JSON data into a collection of holds.
        */
        UKControllerPlugin::Hold::HoldingDataCollection BuildHoldingData(nlohmann::json data)
        {
            HoldingDataCollection collection;

            // If not object, nothing to do
            if (!data.is_array()) {
                LogWarning("Holding data is invalid");
                return collection;
            }

            // Check valid and add
            for (nlohmann::json::const_iterator it = data.cbegin(); it != data.cend(); ++it) {
                if (CheckValid(*it)) {
                    collection.Add(
                        {
                            it->at("id"),
                            it->at("fix"),
                            it->at("description"),
                            it->at("minimum_altitude"),
                            it->at("maximum_altitude"),
                            it->at("inbound_heading"),
                            it->at("turn_direction") == "left" ?
                                HoldingData::TURN_DIRECTION_LEFT : HoldingData::TURN_DIRECTION_RIGHT
                        }
                    );
                } else {
                    std::string holdId = "unknown";
                    if (data.find("id") != data.end() && data.at("id").is_number_integer()) {
                        holdId = data.at("id");
                    }
                    LogWarning("Invalid hold data for " + holdId);
                }
            }

            LogInfo("Added holding data for " + std::to_string(collection.Count()) + " holds");
            return collection;
        }

        /*
            Checks that the holding data is valid.
        */
        bool CheckValid(nlohmann::json data)
        {
            return data.is_object() &&
                data.find("id") != data.end() &&
                data.at("id").is_number_integer() &&
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
