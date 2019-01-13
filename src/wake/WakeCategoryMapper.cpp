
#include "pch/stdafx.h"
#include "wake/WakeCategoryMapper.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;

namespace UKControllerPlugin {
    namespace Wake {

        void WakeCategoryMapper::AddTypeMapping(std::string type, std::string category)
        {
            if (this->typeMap.count(type) > 0) {
                return;
            }

            this->typeMap[type] = category;
        }

        /*
            Return count
        */
        int WakeCategoryMapper::Count(void) const
        {
            return this->typeMap.size();
        }

        /*
            Maps the flightplan to a category or returns the ICAO category if no conversion.
        */
        std::string WakeCategoryMapper::MapFlightplanToCategory(const EuroScopeCFlightPlanInterface & flightPlan) const
        {
            return this->typeMap.count(flightPlan.GetAircraftType()) ?
                this->typeMap.at(flightPlan.GetAircraftType()) :
                flightPlan.GetIcaoWakeCategory();
        }

    }  // namespace Wake
}  // namespace UKControllerPlugin
