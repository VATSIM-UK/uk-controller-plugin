#include "pch/stdafx.h"
#include "hold/HoldEventHandler.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope//EuroScopeCRadarTargetInterface.h"
#include "hold/HoldManager.h"
#include "plugin/PopupMenuItem.h"
#include "euroscope/EuroscopeSectorFileElementInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Navaids::NavaidCollection;

namespace UKControllerPlugin {
    namespace Hold {

        HoldEventHandler::HoldEventHandler(
            HoldManager & holdManager,
            const NavaidCollection& navaids,
            EuroscopePluginLoopbackInterface & plugin,
            const int popupMenuItemId
        )
            : holdManager(holdManager), navaids(navaids), plugin(plugin),
            popupMenuItemId(popupMenuItemId)
        {

        }

        void HoldEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            // Nothing to do here
        }

        /*
            When a flightplan disconnects, remove that aircraft from the hold.
        */
        void HoldEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->holdManager.RemoveAircraftFromAnyHold(flightPlan.GetCallsign());
        }

        void HoldEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {
            // Nothing to do here
        }

        /*
            Return the description of the hold tag item
        */
        std::string HoldEventHandler::GetTagItemDescription(void) const
        {
            return "Selected Hold";
        }

        /*
            Return the value of the hold tag item
        */
        std::string HoldEventHandler::GetTagItemData(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            std::shared_ptr<HoldingAircraft> aircraft = this->holdManager.GetHoldingAircraft(flightPlan.GetCallsign());
            if (!aircraft) {
                return this->noHold;
            }

            return "H" + aircraft->GetAssignedHold();
        }

        void HoldEventHandler::TimedEventTrigger(void)
        {
            this->plugin.ApplyFunctionToAllFlightplans(
                [this](std::shared_ptr<EuroScopeCFlightPlanInterface> fp, std::shared_ptr<EuroScopeCRadarTargetInterface> rt) {
                    
                    for (
                        NavaidCollection::const_iterator navaids = this->navaids.cbegin();
                        navaids != this->navaids.cend();
                        ++navaids
                    ) {
                        if (rt->GetPosition().DistanceTo(navaids->coordinates) <= this->proximityDistance) {
                            this->holdManager.AddAircraftToProximityHold(*fp, navaids->identifier);
                        } else {
                            this->holdManager.RemoveAircraftFromProximityHold(fp->GetCallsign(), navaids->identifier);
                        };
                    }

                }
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
