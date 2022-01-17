#include "FlightplanWakeCategoryMapper.h"
#include "aircraft/AircraftType.h"
#include "aircraft/AircraftTypeMapperInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "wake/WakeCategory.h"
#include "wake/WakeScheme.h"

namespace UKControllerPlugin::Wake {

    FlightplanWakeCategoryMapper::FlightplanWakeCategoryMapper(
        const WakeScheme& scheme, const Aircraft::AircraftTypeMapperInterface& aircraftTypes)
        : scheme(scheme), aircraftTypes(aircraftTypes)
    {
    }

    auto FlightplanWakeCategoryMapper::MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
        -> std::shared_ptr<WakeCategory>
    {
        const auto aircraftTypeString = flightplan.GetAircraftType();
        // If cached, re-use
        if (this->cache.count(aircraftTypeString) != 0) {
            return this->cache.at(aircraftTypeString);
        }

        // If no aircraft type, no category.
        const auto aircraftType = aircraftTypes.MapForFlightplan(flightplan);
        if (!aircraftType) {
            this->cache[aircraftTypeString] = nullptr;
            return nullptr;
        }

        // Map to the wake category
        const auto wakeCategory = std::find_if(
            scheme.Categories().cbegin(),
            scheme.Categories().cend(),
            [&aircraftType](const std::shared_ptr<WakeCategory>& category) -> bool {
                return aircraftType->WakeCategories().find(category->Id()) != aircraftType->WakeCategories().cend();
            });

        this->cache[aircraftTypeString] = wakeCategory == scheme.Categories().cend() ? nullptr : *wakeCategory;
        return this->cache[aircraftTypeString];
    }
} // namespace UKControllerPlugin::Wake
