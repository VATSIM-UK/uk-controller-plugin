#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface;

namespace UKControllerPlugin {
    namespace Flightplan {

        /*
            Returns the number of registered handlers.
        */
        int FlightPlanEventHandlerCollection::CountHandlers(void) const
        {
            return this->handlerList.size();
        }

        /*
            Called whenever there's a flightplan or flightplan controller data event.
        */
        void FlightPlanEventHandlerCollection::FlightPlanEvent(
            EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget) const
        {
            // Loop through the handlers and call their handling function.
            for (std::list<std::shared_ptr<FlightPlanEventHandlerInterface>>::const_iterator it =
                     this->handlerList.cbegin();
                 it != this->handlerList.cend();
                 ++it) {
                (*it)->FlightPlanEvent(flightPlan, radarTarget);
            }
        }

        void FlightPlanEventHandlerCollection::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget, int dataType) const
        {
            // Loop through the handlers and call their handling function.
            for (std::list<std::shared_ptr<FlightPlanEventHandlerInterface>>::const_iterator it =
                     this->handlerList.cbegin();
                 it != this->handlerList.cend();
                 ++it) {
                (*it)->ControllerFlightPlanDataEvent(flightPlan, dataType);
            }
        }

        void
        FlightPlanEventHandlerCollection::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan) const
        {
            // Loop through the handlers and call their handling function.
            for (std::list<std::shared_ptr<FlightPlanEventHandlerInterface>>::const_iterator it =
                     this->handlerList.cbegin();
                 it != this->handlerList.cend();
                 ++it) {
                (*it)->FlightPlanDisconnectEvent(flightPlan);
            }
        }

        /*
            Registers an object to handle and event.
        */
        void FlightPlanEventHandlerCollection::RegisterHandler(std::shared_ptr<FlightPlanEventHandlerInterface> handler)
        {
            if (std::find(this->handlerList.begin(), this->handlerList.end(), handler) != this->handlerList.end()) {
                return;
            }

            this->handlerList.push_back(handler);
        }
    } // namespace Flightplan
} // namespace UKControllerPlugin
