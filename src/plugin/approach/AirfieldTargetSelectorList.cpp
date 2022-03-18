#include "AirfieldApproachOptions.h"
#include "AirfieldTargetSelectorList.h"
#include "ApproachSequencerDisplayOptions.h"
#include "ApproachSequencerDistanceOptions.h"
#include "ApproachSequencerOptions.h"
#include "list/ListItem.h"

using UKControllerPlugin::List::ListItemCheckedStatus;

namespace UKControllerPlugin::Approach {

    AirfieldTargetSelectorList::AirfieldTargetSelectorList(
        ApproachSequencerOptions& options, std::shared_ptr<const ApproachSequencerDisplayOptions> displayOptions)
        : options(options), displayOptions(std::move(displayOptions))
    {
    }

    auto AirfieldTargetSelectorList::ListColumns() -> int
    {
        return 1;
    }

    auto AirfieldTargetSelectorList::ListName() -> std::string
    {
        return "Target";
    }

    auto AirfieldTargetSelectorList::ListItems() -> std::list<std::shared_ptr<List::ListItem>>
    {
        if (displayOptions->Airfield().empty()) {
            return {};
        }

        std::list<std::shared_ptr<List::ListItem>> items;
        const auto& airfieldOptions = options.Get(displayOptions->Airfield());

        // Wake item
        items.push_back(std::make_shared<List::ListItem>(
            "Wake",
            "",
            false,
            false,
            airfieldOptions.defaultMode == ApproachSequencingMode::WakeTurbulence ? ListItemCheckedStatus::Checked
                                                                                  : ListItemCheckedStatus::NotChecked));

        // Distance items
        for (const auto& distance : DistanceOptions()) {
            const auto selected = airfieldOptions.defaultMode == ApproachSequencingMode::MinimumDistance &&
                                          airfieldOptions.targetDistance == distance
                                      ? ListItemCheckedStatus::Checked
                                      : ListItemCheckedStatus::NotChecked;

            char distanceString[25];                     // NOLINT
            sprintf_s(distanceString, "%.1f", distance); // NOLINT
            items.push_back(std::make_shared<List::ListItem>(distanceString, "", false, false, selected));
        }

        return items;
    }

    void AirfieldTargetSelectorList::ItemSelected(const std::string& item)
    {
        if (displayOptions->Airfield().empty()) {
            return;
        }

        const auto& currentOptions = options.Get(displayOptions->Airfield());
        if (item == "Wake") {
            options.Set(
                displayOptions->Airfield(),
                std::make_shared<AirfieldApproachOptions>(
                    ApproachSequencingMode::WakeTurbulence,
                    currentOptions.targetDistance,
                    currentOptions.minimumSeparationRequirement));

            return;
        }

        options.Set(
            displayOptions->Airfield(),
            std::make_shared<AirfieldApproachOptions>(
                ApproachSequencingMode::MinimumDistance, std::stod(item), currentOptions.minimumSeparationRequirement));
    }
} // namespace UKControllerPlugin::Approach
