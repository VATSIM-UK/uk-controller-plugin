#include "AirfieldApproachOptions.h"
#include "AirfieldMinimumSeparationSelectorList.h"
#include "ApproachSequencerDisplayOptions.h"
#include "ApproachSequencerDistanceOptions.h"
#include "ApproachSequencerOptions.h"
#include "list/ListItem.h"

using UKControllerPlugin::List::ListItemCheckedStatus;

namespace UKControllerPlugin::Approach {

    AirfieldMinimumSeparationSelectorList::AirfieldMinimumSeparationSelectorList(
        ApproachSequencerOptions& options, std::shared_ptr<const ApproachSequencerDisplayOptions> displayOptions)
        : options(options), displayOptions(std::move(displayOptions))
    {
    }

    auto AirfieldMinimumSeparationSelectorList::ListColumns() -> int
    {
        return 1;
    }

    auto AirfieldMinimumSeparationSelectorList::ListName() -> std::string
    {
        return "Separation";
    }

    auto AirfieldMinimumSeparationSelectorList::ListItems() -> std::list<std::shared_ptr<List::ListItem>>
    {
        if (displayOptions->Airfield().empty()) {
            return {};
        }

        std::list<std::shared_ptr<List::ListItem>> items;
        const auto& airfieldOptions = options.Get(displayOptions->Airfield());

        // Distance items
        for (const auto& distance : DistanceOptions()) {
            const auto selected = airfieldOptions.minimumSeparationRequirement == distance
                                      ? ListItemCheckedStatus::Checked
                                      : ListItemCheckedStatus::NotChecked;

            char distanceString[25];                     // NOLINT
            sprintf_s(distanceString, "%.1f", distance); // NOLINT
            items.push_back(std::make_shared<List::ListItem>(distanceString, "", false, false, selected));
        }

        return items;
    }

    void AirfieldMinimumSeparationSelectorList::ItemSelected(const std::string& item)
    {
        if (displayOptions->Airfield().empty()) {
            return;
        }

        const auto& currentOptions = options.Get(displayOptions->Airfield());
        options.Set(
            displayOptions->Airfield(),
            std::make_shared<AirfieldApproachOptions>(
                currentOptions.defaultMode, currentOptions.targetDistance, std::stod(item)));
    }
} // namespace UKControllerPlugin::Approach
