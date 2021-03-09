#include "pch/stdafx.h"
#include "hold/DeemedSeparatedHoldSerializer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Hold {

        void deemed_separated_from_json(const nlohmann::json& json,
            std::set<std::unique_ptr<DeemedSeparatedHold>>& deemedSeparatedHolds
        ) {
            for (nlohmann::json::const_iterator it = json.cbegin(); it != json.cend(); ++it) {
                if (!ValidDeemedSeparatedData(*it)) {
                    LogWarning("Invalid deemed separated holds data");
                    continue;
                }

                deemedSeparatedHolds.insert(
                    std::make_unique<DeemedSeparatedHold>(
                        it->at("hold_id").get<int>(),
                        it->at("vsl_insert_distance").get<int>()
                    )
                );
            }
        }

        bool ValidDeemedSeparatedData(const nlohmann::json& json)
        {
            return json.is_object() &&
                json.contains("hold_id") &&
                json.at("hold_id").is_number_integer() &&
                json.contains("vsl_insert_distance") &&
                json.at("vsl_insert_distance").is_number_integer();
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
