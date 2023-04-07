#pragma once
#include "list/ListItemProviderInterface.h"

namespace UKControllerPlugin::Approach {
    class ApproachSequencerOptions;
    class ApproachSequencerDisplayOptions;

    /**
     * Selects the target for a given airfield
     */
    class AirfieldTargetSelectorList : public List::ListItemProviderInterface
    {
        public:
        AirfieldTargetSelectorList(
            ApproachSequencerOptions& options, std::shared_ptr<const ApproachSequencerDisplayOptions> displayOptions);
        auto ListColumns() -> int override;
        auto ListName() -> std::string override;
        auto ListItems() -> std::list<std::shared_ptr<List::ListItem>> override;
        void ItemSelected(const std::string& item) override;

        private:
        // A place for saving options
        ApproachSequencerOptions& options;

        // Options from the display
        std::shared_ptr<const ApproachSequencerDisplayOptions> displayOptions;
    };
} // namespace UKControllerPlugin::Approach
