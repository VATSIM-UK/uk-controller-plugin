#include "ApproachSequence.h"
#include "ApproachSequencedAircraft.h"
#include "ApproachSequencer.h"
#include "ApproachSequencerDisplayOptions.h"
#include "TargetSelectorList.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "list/ListItem.h"

using UKControllerPlugin::List::ListItemCheckedStatus;

namespace UKControllerPlugin::Approach {

    TargetSelectorList::TargetSelectorList(
        ApproachSequencer& sequencer,
        std::shared_ptr<ApproachSequencerDisplayOptions> options,
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : sequencer(sequencer), options(std::move(options)), plugin(plugin)
    {
    }

    auto TargetSelectorList::ListName() -> std::string
    {
        return "Target Spacing";
    }

    auto TargetSelectorList::ListColumns() -> int
    {
        return 1;
    }

    void TargetSelectorList::ItemSelected(const std::string& item)
    {
        auto flightplan = plugin.GetSelectedFlightplan();
        if (!flightplan) {
            return;
        }

        if (item == "Wake") {
            sequencer.GetForAirfield(options->Airfield())
                .Get(flightplan->GetCallsign())
                ->Mode(ApproachSequencingMode::WakeTurbulence);
            return;
        }

        auto aircraft = sequencer.GetForAirfield(options->Airfield()).Get(flightplan->GetCallsign());
        aircraft->Mode(ApproachSequencingMode::MinimumDistance);
        aircraft->ExpectedDistance(std::stod(item));
    }

    auto TargetSelectorList::ListItems() -> std::list<std::shared_ptr<List::ListItem>>
    {
        auto flightplan = plugin.GetSelectedFlightplan();
        if (!flightplan || options->Airfield().empty() ||
            !sequencer.GetForAirfield(options->Airfield()).Get(flightplan->GetCallsign())) {
            return {};
        }

        auto aircraft = sequencer.GetForAirfield(options->Airfield()).Get(flightplan->GetCallsign());
        std::list<std::shared_ptr<List::ListItem>> items;

        // Wake item
        items.push_back(std::make_shared<List::ListItem>(
            "Wake",
            "",
            false,
            false,
            aircraft->Mode() == ApproachSequencingMode::WakeTurbulence ? ListItemCheckedStatus::Checked
                                                                       : ListItemCheckedStatus::NotChecked));

        // Distance items
        for (const auto& distance : distances) {
            const auto selected =
                aircraft->Mode() == ApproachSequencingMode::MinimumDistance && aircraft->ExpectedDistance() == distance
                    ? ListItemCheckedStatus::Checked
                    : ListItemCheckedStatus::NotChecked;

            char distanceString[25];                     // NOLINT
            sprintf_s(distanceString, "%.1f", distance); // NOLINT
            items.push_back(std::make_shared<List::ListItem>(distanceString, "", false, false, selected));
        }

        return items;
    }
} // namespace UKControllerPlugin::Approach
