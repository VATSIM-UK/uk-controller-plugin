#include "releases/EnrouteReleaseTypesSerializer.h"

namespace UKControllerPlugin {
    namespace Releases {

        const EnrouteReleaseType releaseTypeInvalid = {};

        void from_json(const nlohmann::json& json, std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes>& releases)
        {
            if (!DependencyValid(json)) {
                LogError("Enroute release dependency is not valid");
                return;
            }

            for (nlohmann::json::const_iterator it = json.cbegin(); it != json.cend(); ++it) {
                if (!JsonValid(*it)) {
                    LogError("Invalid enroute release " + it->dump());
                    continue;
                }

                releases.insert(
                    {it->at("id").get<int>(),
                     it->at("tag_string").get<std::string>(),
                     it->at("description").get<std::string>()});
            }

            LogInfo("Loaded " + std::to_string(releases.size()) + " enroute release types");
        }

        bool DependencyValid(const nlohmann::json& data)
        {
            return data.is_array();
        }

        bool JsonValid(const nlohmann::json& data)
        {
            return data.is_object() && data.contains("id") && data.at("id").is_number_integer() &&
                   data.contains("tag_string") && data.at("tag_string").is_string() && data.contains("description") &&
                   data.at("description").is_string();
        }
    } // namespace Releases
} // namespace UKControllerPlugin
