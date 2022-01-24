#include "FlightplanWakeCategoryMapper.h"
#include "WakeCalculatorOptions.h"
#include "WakeScheme.h"
#include "WakeSchemeCollection.h"
#include "WakeSchemeProvider.h"
#include "list/ListItem.h"
#include "list/ListItemCheckedStatus.h"

namespace UKControllerPlugin::Wake {

    WakeSchemeProvider::WakeSchemeProvider(
        std::shared_ptr<WakeCalculatorOptions> options,
        const WakeSchemeCollection& schemes,
        const Aircraft::AircraftTypeMapperInterface& aircraftTypeMapper)
        : options(options), schemes(schemes), aircraftTypeMapper(aircraftTypeMapper)
    {
    }

    auto WakeSchemeProvider::ListColumns() -> int
    {
        return 1;
    }

    auto WakeSchemeProvider::ListName() -> std::string
    {
        return "Scheme";
    }

    auto WakeSchemeProvider::ListItems() -> std::list<std::shared_ptr<List::ListItem>>
    {
        std::list<std::shared_ptr<List::ListItem>> items;
        schemes.ForEach([&items](const WakeScheme& scheme) {
            items.push_back(std::make_shared<List::ListItem>(
                scheme.Name(), "", false, false, List::ListItemCheckedStatus::NoCheckbox));
        });
        return items;
    }

    void WakeSchemeProvider::ItemSelected(const std::string& item)
    {
        const auto scheme = schemes.GetByName(item);
        if (scheme == nullptr) {
            LogError("Selected invalid wake scheme");
            return;
        }

        options->Scheme(scheme->Name());
        options->SchemeMapper(std::make_shared<FlightplanWakeCategoryMapper>(*scheme, aircraftTypeMapper));
    }
} // namespace UKControllerPlugin::Wake
