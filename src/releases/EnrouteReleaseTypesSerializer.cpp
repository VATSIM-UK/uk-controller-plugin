#include "pch/stdafx.h"
#include "releases/EnrouteReleaseTypesSerializer.h"

namespace UKControllerPlugin {
    namespace Releases {

        const EnrouteReleaseType releaseTypeInvalid = {};

        void from_json(const nlohmann::json& json, EnrouteReleaseType& releaseType)
        {
            if (!JsonValid(json)) {
                releaseType = releaseTypeInvalid;
                return;
            }

            json.at("id").get_to(releaseType.id);
            json.at("tag_string").get_to(releaseType.tagString);
            json.at("description").get_to(releaseType.description);
        }

        bool JsonValid(const nlohmann::json& data)
        {
            return data.is_object() &&
                data.contains("id") &&
                data.at("id").is_number_integer() &&
                data.contains("tag_string") &&
                data.at("tag_string").is_string() &&
                data.contains("description") &&
                data.at("description").is_string();
        }
    }  // namespace Releases
}  // namespace UKControllerPlugin
