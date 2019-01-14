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
            if (!data.is_object()) {
                LogWarning("Holding data is invalid");
                return collection;
            }

            // Check valid and add
            for (nlohmann::json::const_iterator it = data.cbegin(); it != data.cend(); ++it) {
                if (CheckValid(*it)) {
                    collection.Add(
                        {
                            it.key(),
                            it->at("minimum"),
                            it->at("maximum"),
                            it->at("inbound"),
                            it->at("direction") == "left" ?
                                HoldingData::TURN_DIRECTION_LEFT : HoldingData::TURN_DIRECTION_RIGHT
                        }
                    );
                } else {
                    LogWarning("Invalid hold data for " + it.key());
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
                data.find("minimum") != data.end() &&
                data.at("minimum").is_number_integer() &&
                data.find("maximum") != data.end() &&
                data.at("maximum").is_number_integer() &&
                data.find("inbound") != data.end() &&
                data.at("inbound").is_number_integer() &&
                data.find("direction") != data.end() &&
                data.at("direction").is_string() &&
                (data.at("direction") == "left" || data.at("direction") == "right");
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin