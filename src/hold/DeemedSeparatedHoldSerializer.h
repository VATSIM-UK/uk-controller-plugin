#pragma once
#include "hold/DeemedSeparatedHold.h"
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Hold {
        void deemed_separated_from_json(
            const nlohmann::json & json,
            std::set<std::unique_ptr<DeemedSeparatedHold>> & deemedSeparatedHolds
        );
        bool ValidDeemedSeparatedData(const nlohmann::json & json);
    }  // namespace Hold
}  // namespace UKControllerPlugin
