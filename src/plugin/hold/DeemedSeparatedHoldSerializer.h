#pragma once
namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin {
    namespace Hold {
        struct DeemedSeparatedHold;

        void deemed_separated_from_json(
            const nlohmann::json& json, std::set<std::unique_ptr<DeemedSeparatedHold>>& deemedSeparatedHolds);
        bool ValidDeemedSeparatedData(const nlohmann::json& json);
    } // namespace Hold
} // namespace UKControllerPlugin
