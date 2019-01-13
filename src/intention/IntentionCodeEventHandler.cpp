#include "pch/stdafx.h"
#include "intention/IntentionCodeEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "intention/IntentionCodeData.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::IntentionCode::IntentionCodeCache;
using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

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
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
