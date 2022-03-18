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
        [[nodiscard]] auto CountAirfieldOptions() const -> size_t;
        void Set(const std::string& airfield, std::shared_ptr<AirfieldApproachOptions> options);
        [[nodiscard]] auto Get(const std::string& airfield) -> const AirfieldApproachOptions&;
        [[nodiscard]] auto HasAirfield(const std::string& airfield) const -> bool;
        void ForEach(const std::function<void(const std::string&, const AirfieldApproachOptions&)> callback) const;

        private:
        // The options for the airfield
        std::map<std::string, std::shared_ptr<AirfieldApproachOptions>> airfieldOptions;
    };
} // namespace UKControllerPlugin::Approach
