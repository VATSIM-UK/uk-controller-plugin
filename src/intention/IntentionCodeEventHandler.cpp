#include "pch/stdafx.h"
#include "intention/IntentionCodeEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "intention/IntentionCodeData.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"
#include "euroscope/EuroScopeCControllerInterface.h"

using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::IntentionCode::IntentionCodeCache;
using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {
        IntentionCodeEventHandler::IntentionCodeEventHandler(
            IntentionCodeGenerator intention,
            IntentionCodeCache codeCache
        )
            : intention(std::move(intention)), codeCache(codeCache)
        {

        }

        /*
            Nothing to do here.
        */
        void IntentionCodeEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {

        }

        /*
            Respond to flightplan updates - invalidate the intention code cache.
        */
        void IntentionCodeEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            this->codeCache.UnregisterAircraft(flightPlan.GetCallsign());
        }

        /*
            Respond to flightplans disconnecting - invalidate the intention code cache.
        */
        void IntentionCodeEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->codeCache.UnregisterAircraft(flightPlan.GetCallsign());
        }

        /*
            Returns the description of the TagItem.
        */
        std::string IntentionCodeEventHandler::GetTagItemDescription(void) const
        {
            return "UKCP Intention Code";
        }

        /*
            Returns the intention code.
        */
        std::string IntentionCodeEventHandler::GetTagItemData(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            // If we have it cached, then use the cached value
            EuroscopeExtractedRouteInterface extractedRoute = flightPlan.GetExtractedRoute();
            if (
                this->codeCache.HasIntentionCodeForAircraft(flightPlan.GetCallsign()) &&
                this->codeCache.IntentionCodeValid(flightPlan.GetCallsign(), extractedRoute)
                ) {
                return this->codeCache.GetIntentionCodeForAircraft(flightPlan.GetCallsign());
            }

            // Generate the code and then cache it
            IntentionCodeData data = this->intention.GetIntentionCodeForFlightplan(
                flightPlan.GetCallsign(),
                flightPlan.GetOrigin(),
                flightPlan.GetDestination(),
                extractedRoute,
                flightPlan.GetCruiseLevel()
            );

            this->codeCache.RegisterAircraft(flightPlan.GetCallsign(), data);
            return data.intentionCode;
        }

        /*
            Clear the code cache if the user logs on with a different controller position.
        */
        void IntentionCodeEventHandler::ControllerUpdateEvent(EuroScopeCControllerInterface& controller)
        {
            if (controller.IsCurrentUser() && controller.GetCallsign() != this->intention.GetUserControllerPosition()) {
                this->codeCache.Clear();
            }
        }

        void IntentionCodeEventHandler::ControllerDisconnectEvent(EuroScopeCControllerInterface& controller)
        {
            // Nothing to do here, we only care about the current user
        }

        /*
            Clear the code cache if the user disconnects
        */
        void IntentionCodeEventHandler::SelfDisconnectEvent(void)
        {
            this->codeCache.Clear();
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
