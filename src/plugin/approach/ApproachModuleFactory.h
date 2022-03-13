#pragma once

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;

    /**
     * All the approach module dependencies.
     */
    class ApproachModuleFactory
    {
        public:
        ApproachModuleFactory();
        ~ApproachModuleFactory();
        [[nodiscard]] auto Sequencer() -> ApproachSequencer&;

        private:
        // The approach sequencer
        std::unique_ptr<ApproachSequencer> sequencer;
    };
} // namespace UKControllerPlugin::Approach
