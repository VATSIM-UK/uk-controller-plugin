#pragma once

namespace UKControllerPlugin {
    namespace IntentionCode {
        class SectorExitRepository;
    } // namespace IntentionCode
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Creates the sector exit repository
        */
        class SectorExitRepositoryFactory
        {
            public:
            static std::unique_ptr<UKControllerPlugin::IntentionCode::SectorExitRepository> Create();
        };
    } // namespace IntentionCode
} // namespace UKControllerPlugin
