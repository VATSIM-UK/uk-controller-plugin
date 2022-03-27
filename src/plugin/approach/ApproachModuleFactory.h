#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;
    class ApproachSequencerOptions;
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
        [[nodiscard]] auto SequencerOptions() -> ApproachSequencerOptions&;
        [[nodiscard]] auto SpacingCalculator(const Bootstrap::PersistenceContainer& container)
            -> ApproachSpacingCalculator&;

        private:
        // The approach sequencer
        std::unique_ptr<ApproachSequencer> sequencer;

        // The approach sequencer options
        std::unique_ptr<ApproachSequencerOptions> sequencerOptions;

        // For calculating the spacing required between two aircraft
        std::unique_ptr<ApproachSpacingCalculator> spacingCalculator;
    };
} // namespace UKControllerPlugin::Approach
