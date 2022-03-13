#pragma once
#include "list/ListItemProviderInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;
    class ApproachSequencerDisplayOptions;

    /**
     * A list for selecting the intended target distance on approach.
     */
    class TargetSelectorList : public List::ListItemProviderInterface
    {
        public:
        TargetSelectorList(
            ApproachSequencer& sequencer,
            std::shared_ptr<ApproachSequencerDisplayOptions> options,
            Euroscope ::EuroscopePluginLoopbackInterface& plugin);
        [[nodiscard]] auto ListName() -> std::string override;
        [[nodiscard]] auto ListColumns() -> int override;
        [[nodiscard]] auto ListItems() -> std::list<std::shared_ptr<List::ListItem>> override;
        void ItemSelected(const std::string& item) override;

        private:
        // Manages sequences
        ApproachSequencer& sequencer;

        // Display options for the sequencer
        std::shared_ptr<ApproachSequencerDisplayOptions> options;

        // Plugin instance
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The distances we're allowed to aim for
        const std::set<double> distances{
            2.5,
            3.0,
            4.0,
            5.0,
            7.0,
            8.0,
            9.0,
            10.0,
            11.0,
            12.0,
            13.0,
            14.0,
            15.0,
        };
    };
} // namespace UKControllerPlugin::Approach
