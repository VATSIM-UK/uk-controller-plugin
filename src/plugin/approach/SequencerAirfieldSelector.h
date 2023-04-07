#pragma once
#include "list/ListItemProviderInterface.h"

namespace UKControllerPlugin::Airfield {
    class AirfieldCollection;
} // namespace UKControllerPlugin::Airfield

namespace UKControllerPlugin::Approach {
    class ApproachSequencerDisplayOptions;

    /**
     * Selects the airfield for the approach sequencer.
     */
    class SequencerAirfieldSelector : public List::ListItemProviderInterface
    {
        public:
        SequencerAirfieldSelector(
            std::shared_ptr<ApproachSequencerDisplayOptions> options, const Airfield::AirfieldCollection& airfields);
        auto ListColumns() -> int override;
        auto ListName() -> std::string override;
        auto ListItems() -> std::list<std::shared_ptr<List::ListItem>> override;
        void ItemSelected(const std::string& item) override;

        private:
        // The options
        std::shared_ptr<ApproachSequencerDisplayOptions> options;

        // All the airfields
        const Airfield::AirfieldCollection& airfields;
    };
} // namespace UKControllerPlugin::Approach
