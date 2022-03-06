#pragma once

#include <string>
namespace UKControllerPlugin::Approach {
    /**
     * Options for the approach sequencer display.
     */
    class ApproachSequencerDisplayOptions
    {
        public:
        [[nodiscard]] auto Airfield() const -> const std::string&;
        void Airfield(const std::string& airfield);

        private:
        // The airfield we are displaying for
        std::string airfield;
    };
} // namespace UKControllerPlugin::Approach
