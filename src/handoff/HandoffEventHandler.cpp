#include "pch/stdafx.h"
#include "handoff/HandoffEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin {
    namespace Handoff {

        HandoffEventHandler::HandoffEventHandler(
            const HandoffCollection& handoffs,
            const ActiveCallsignCollection& callsigns
        ) : handoffs(std::move(handoffs)), callsigns(std::move(callsigns))
        {

        }

        /*
            Add an item to the cache.
        */
        void HandoffEventHandler::AddCachedItem(std::string callsign, std::string item)
        {
            this->cache[callsign] = item;
        }

        /*
            Get the cached item
        */
        std::string HandoffEventHandler::GetCachedItem(std::string callsign) const
        {
            return this->cache.count(callsign) ? this->cache.at(callsign) : this->DEFAULT_TAG_VALUE;
        }

        std::string HandoffEventHandler::GetTagItemDescription(void) const
        {
            return "Departure Handoff Next Controller";
        }

        std::string HandoffEventHandler::GetTagItemData(
            EuroScopeCFlightPlanInterface& flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget
        ) {
            if (this->cache.count(flightPlan.GetCallsign())) {
                return this->cache.at(flightPlan.GetCallsign());
            }

            ControllerPositionHierarchy controllers = this->handoffs.GetSidHandoffOrder(
                flightPlan.GetOrigin(),
                flightPlan.GetSidName()
            );

            for (
                ControllerPositionHierarchy::const_iterator it = controllers.cbegin();
                it != controllers.cend();
                ++it
            ){
                if (this->callsigns.PositionActive(it->get().GetCallsign())) {
                    char frequencyString[24];
                    sprintf_s(frequencyString, "%.3f", it->get().GetFrequency());
                    this->cache[flightPlan.GetCallsign()] = std::string(frequencyString);
                    return this->cache[flightPlan.GetCallsign()];
                }
            }

            this->cache[flightPlan.GetCallsign()] = this->DEFAULT_TAG_VALUE;
            return this->DEFAULT_TAG_VALUE;
        }

        void HandoffEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface& flightPlan,
            EuroScopeCRadarTargetInterface& radarTarget
        ) {
        }

        void HandoffEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
        {
        }

        void HandoffEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
        {
        }
    }  // namespace Handoff
}  // namespace UKControllerPlugin
