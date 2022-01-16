#include "BlockedHoldLevelRestriction.h"
#include "BlockedHoldLevelRestrictionSerializer.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Make the class from JSON
        */
        void from_json(const nlohmann::json& json, std::unique_ptr<BlockedHoldLevelRestriction>& restriction)
        {
            if (!ValidateLevelRestrictionData(json)) {
                restriction.reset(nullptr);
                LogWarning("Invalid blocked hold level restriction");
                return;
            }

            restriction.reset(new BlockedHoldLevelRestriction(json.at("levels")));
        }

        /*
            Serialize the class to JSON
        */
        void to_json(nlohmann::json& json, const std::unique_ptr<BlockedHoldLevelRestriction>& restriction)
        {
            json["type"] = "level-block";
            json["levels"] = restriction->GetLevels();
        }

        /*
            Check that the JSON data is valid
        */
        bool ValidateLevelRestrictionData(const nlohmann::json& json)
        {
            if (json.find("levels") == json.cend() || !json.at("levels").is_array() || json.at("levels").size() == 0) {
                return false;
            }

            for (nlohmann::json::const_iterator it = json.at("levels").cbegin(); it != json.at("levels").cend(); ++it) {
                if (!it->is_number_integer()) {
                    return false;
                }
            }

            return true;
        }
    } // namespace Hold
} // namespace UKControllerPlugin
