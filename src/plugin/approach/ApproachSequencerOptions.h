#pragma once
#include "ApproachSequencingMode.h"

namespace UKControllerPlugin::Approach {
    struct AirfieldApproachOptions;

    /**
     * All the options for the approach sequencer that aren't specific
     * to the display itself
     */
    class ApproachSequencerOptions
    {
        public:
        void Set(const std::string& airfield, std::shared_ptr<AirfieldApproachOptions> options);
        [[nodiscard]] auto Get(const std::string& airfield) -> const AirfieldApproachOptions&;

        private:
        // The options for the airfield
        std::map<std::string, std::shared_ptr<AirfieldApproachOptions>> airfieldOptions;
    };
} // namespace UKControllerPlugin::Approach