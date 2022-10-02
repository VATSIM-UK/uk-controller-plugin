#pragma once

// Forward Declarations
namespace UKControllerPlugin {
    namespace IntentionCode {
        class IntentionCodeGenerator;
        class SectorExitRepository;
    } // namespace IntentionCode
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Class for creating the intention code generator.
        */
        class IntentionCodeFactory
        {
            public:
            static std::unique_ptr<IntentionCodeGenerator>
            Create(UKControllerPlugin::IntentionCode::SectorExitRepository& exitPoints);
        };
    } // namespace IntentionCode
} // namespace UKControllerPlugin
