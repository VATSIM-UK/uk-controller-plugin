#include "pch/stdafx.h"
#include "hold/HoldRestrictionSerializer.h"
#include "hold/BlockedHoldLevelRestrictionSerializer.h"
#include "hold/BlockedHoldLevelRestriction.h"
#include "hold/MinStackHoldLevelRestriction.h"
#include "hold/MinStackHoldLevelRestrictionSerializer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Hold {

        const std::string levelBlockRestriction = "level-block";

        const std::string minimumLevelRestriction = "minimum-level";

        void hold_restriction_from_json(
            const nlohmann::json & json,
            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> & restrictions,
            const PersistenceContainer & container
        ) {
            for (nlohmann::json::const_iterator it = json.cbegin(); it != json.cend(); ++it) {
                if (!ValidRestrictionData(*it)) {
                    continue;
                }

                std::unique_ptr<AbstractHoldLevelRestriction> restriction;
                if (it->at("type") == levelBlockRestriction) {
                   restriction = it->get<std::unique_ptr<BlockedHoldLevelRestriction>>();
                }

                // Not yet implemented
                if (it->at("type") == minimumLevelRestriction) {
                    restriction = MinStackRestrictionFromJson(*it, container);
                }

                if (!restriction) {
                    LogWarning("Invalid hold restriction detected");
                    continue;
                }

                restrictions.insert(std::move(restriction));
            }
        }

        bool ValidRestrictionData(const nlohmann::json & json)
        {
            return json.find("type") != json.cend() &&
                json.at("type").is_string() &&
                (json.at("type") == levelBlockRestriction || json.at("type") == minimumLevelRestriction);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
