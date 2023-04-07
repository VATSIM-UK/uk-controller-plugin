#include "ApproachSequencerDisplayOptions.h"
#include "SequencerAirfieldSelector.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "list/ListItem.h"

namespace UKControllerPlugin::Approach {

    SequencerAirfieldSelector::SequencerAirfieldSelector(
        std::shared_ptr<ApproachSequencerDisplayOptions> options, const Airfield::AirfieldCollection& airfields)
        : options(std::move(options)), airfields(airfields)
    {
    }

    auto SequencerAirfieldSelector::ListColumns() -> int
    {
        return 1;
    }

    auto SequencerAirfieldSelector::ListName() -> std::string
    {
        return "Select Airfield";
    }

    auto SequencerAirfieldSelector::ListItems() -> std::list<std::shared_ptr<List::ListItem>>
    {
        std::set<std::string> airfieldStrings;
        airfields.ForEach(
            [&airfieldStrings](const Airfield::AirfieldModel& airfield) { airfieldStrings.insert(airfield.Icao()); });

        std::list<std::shared_ptr<List::ListItem>> items;
        for (const auto& airfield : airfieldStrings) {
            items.push_back(
                std::make_shared<List::ListItem>(airfield, "", false, false, List::ListItemCheckedStatus::NoCheckbox));
        }

        return items;
    }

    void SequencerAirfieldSelector::ItemSelected(const std::string& item)
    {
        this->options->Airfield(item);
    }
} // namespace UKControllerPlugin::Approach
