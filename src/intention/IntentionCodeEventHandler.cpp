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
using UKControllerPlugin::Tag::TagData;

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
        std::string IntentionCodeEventHandler::GetTagItemDescription(int tagItemId) const
        {
            return "UKCP Intention Code";
        }

        /*
            Returns the intention code.
        */
        void IntentionCodeEventHandler::SetTagItemData(TagData& tagData)
        {
            // If we have it cached, then use the cached value
            EuroscopeExtractedRouteInterface extractedRoute = tagData.flightPlan.GetExtractedRoute();
            if (
                this->codeCache.HasIntentionCodeForAircraft(tagData.flightPlan.GetCallsign()) &&
                this->codeCache.IntentionCodeValid(tagData.flightPlan.GetCallsign(), extractedRoute)
            ) {
                tagData.SetItemString(
                    this->codeCache.GetIntentionCodeForAircraft(tagData.flightPlan.GetCallsign())
                );
                return;
            }

            // Generate the code and then cache it
            IntentionCodeData data = this->intention.GetIntentionCodeForFlightplan(
                tagData.flightPlan.GetCallsign(),
                tagData.flightPlan.GetOrigin(),
                tagData.flightPlan.GetDestination(),
                extractedRoute,
                tagData.flightPlan.GetCruiseLevel()
            );

            this->codeCache.RegisterAircraft(tagData.flightPlan.GetCallsign(), data);
            tagData.SetItemString(data.intentionCode);
        }

        /*
            Clear the code cache if the user logs on with a different controller position.
        */
        void IntentionCodeEventHandler::ControllerUpdateEvent(EuroScopeCControllerInterface& controller)
        {
            if (controller.IsCurrentUser() && controller.GetCallsign() != this->intention.GetUserControllerPosition()) {
                this->intention.SetUserControllerPosition(controller.GetCallsign());
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

        const IntentionCodeGenerator& IntentionCodeEventHandler::GetGenerator() const
        {
            return this->intention;
        }

        const IntentionCodeCache& IntentionCodeEventHandler::GetCache() const
        {
            return this->codeCache;
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
