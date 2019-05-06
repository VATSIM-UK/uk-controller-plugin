#include "pch/stdafx.h"
#include "wake/WakeCategoryEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin {
    namespace Wake {

        WakeCategoryEventHandler::WakeCategoryEventHandler(const WakeCategoryMapper mapper)
            : mapper(mapper)
        {

        }

        /*
            Flightplan has changed, clear the cache
        */
        void WakeCategoryEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            this->cache.erase(flightPlan.GetCallsign());
        }

        /*
            Flightplan has gone, clear the cache
        */
        void WakeCategoryEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->cache.erase(flightPlan.GetCallsign());
        }

        /*
            Nothing to do here
        */
        void WakeCategoryEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {

        }

        std::string WakeCategoryEventHandler::GetTagItemDescription(void) const
        {
            return "Aircraft Type / UK Wake Category";
        }

        /*
            Get item from the cache or generate if not exists
        */
        std::string WakeCategoryEventHandler::GetTagItemData(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            if (this->cache.count(flightPlan.GetCallsign()) == 0) {
                std::string tagString = flightPlan.GetAircraftType()
                    + "/" + this->mapper.MapFlightplanToCategory(flightPlan);

                // 15 characters is the max for tag functions, trim the aircraft type accordingly
                if (tagString.size() > this->maxItemSize) {
                    const unsigned int charactersToTrim = tagString.size() - this->maxItemSize;
                    const std::string aircraftType = flightPlan.GetAircraftType();
                    tagString = aircraftType.substr(0, aircraftType.size() - charactersToTrim) + "/"
                        + this->mapper.MapFlightplanToCategory(flightPlan);
                }

                this->cache[flightPlan.GetCallsign()] = tagString;
            }

            return this->cache.at(flightPlan.GetCallsign());
        }
    }  // namespace Wake
}  // namespace UKControllerPlugin
