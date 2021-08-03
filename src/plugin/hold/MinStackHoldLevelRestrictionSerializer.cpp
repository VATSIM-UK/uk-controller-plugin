#include "pch/pch.h"
#include "hold/MinStackHoldLevelRestrictionSerializer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Hold {

        const std::map<std::string, unsigned int> levelMap = {
            { "MSL", 0 },
            { "MSL+1", 1000 },
            { "MSL+2", 2000 },
            { "MSL+3", 3000 }
        };

        /*
            Returns a restriction
        */
        std::unique_ptr<MinStackHoldLevelRestriction> MinStackRestrictionFromJson(
            const nlohmann::json & json,
            const PersistenceContainer & container
        ) {
            if (!RestrictionJsonValid(json)) {
                LogWarning("Invalid MinStackLevelHoldRestriction: " + json.dump());
                return nullptr;
            }

            unsigned int override = json.count("override")
                ? json.at("override").get<unsigned int>()
                : 0;

            std::string runway = json.count("runway")
                ? json.at("runway").at("designator").get<std::string>()
                : "";

            return std::make_unique<MinStackHoldLevelRestriction>(
                json.at("target").get<std::string>(),
                levelMap.at(json.at("level").get<std::string>()),
                override,
                *container.minStack,
                *container.runways,
                runway
            );
        }


        /*
            Returns true if the holding data is valid
        */
        bool RestrictionJsonValid(const nlohmann::json & json)
        {
            // Check the level
            if (
                !json.count("level") ||
                !json.at("level").is_string() ||
                levelMap.count(json.at("level").get<std::string>()) == 0
            ) {
                return false;
            }

            // Check the target
            if (
                !json.count("target") ||
                !json.at("target").is_string()
            ) {
                return false;
            }

            // Check override
            if (
                json.count("override") &&
                (!json.at("override").is_number_integer() || json.at("override").get<int>() < 0)
            ) {
                return false;
            }

            // Check runway
            if (
                json.count("runway") &&
                (!json.at("runway").is_object() ||
                !json.at("runway").count("designator") ||
                !json.at("runway").at("designator").is_string())
            ) {
                return false;
            }

            return true;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
