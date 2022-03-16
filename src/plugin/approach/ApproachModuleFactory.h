#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;
    class ApproachSpacingCalculator;

    /**
     * All the approach module dependencies.
     */
    class ApproachModuleFactory
    {
        public:
        ApproachModuleFactory();
        ~ApproachModuleFactory();
        [[nodiscard]] auto Sequencer() -> ApproachSequencer&;
        [[nodiscard]] auto SpacingCalculator(const Bootstrap::PersistenceContainer& container)
            -> ApproachSpacingCalculator&;

        private:
        // The approach sequencer
        std::unique_ptr<ApproachSequencer> sequencer;

        // For calculating the spacing required between two aircraft
        std::unique_ptr<ApproachSpacingCalculator> spacingCalculator;
    };
} // namespace UKControllerPlugin::Approach
