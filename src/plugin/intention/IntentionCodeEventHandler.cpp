#include "IntentionCodeCache.h"
#include "IntentionCodeEventHandler.h"
#include "IntentionCodeGenerator.h"
#include "IntentionCodeUpdatedMessage.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "integration/OutboundIntegrationEventHandler.h"
#include "tag/TagData.h"

using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;
using UKControllerPlugin::IntentionCode::IntentionCodeCache;
using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::IntentionCode::IntentionCodeUpdatedMessage;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin::IntentionCode {
    IntentionCodeEventHandler::IntentionCodeEventHandler(
        std::unique_ptr<IntentionCodeGenerator> intention,
        IntentionCodeCache& codeCache,
        Integration::OutboundIntegrationEventHandler& outboundEvent)
        : intention(std::move(intention)), codeCache(codeCache), outboundEvent(outboundEvent)
    {
    }

    IntentionCodeEventHandler::~IntentionCodeEventHandler() = default;
    IntentionCodeEventHandler::IntentionCodeEventHandler(IntentionCodeEventHandler&&) noexcept = default;

    /*
        Nothing to do here.
    */
    void
    IntentionCodeEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
    }

    /*
        Respond to flightplan updates - invalidate the intention code cache.
    */
    void IntentionCodeEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
    {
        this->codeCache.UnregisterAircraft(flightPlan.GetCallsign());
        auto extractedRoute = flightPlan.GetExtractedRoute();
        IntentionCodeData data = this->intention->GetIntentionCodeForFlightplan(
            flightPlan.GetCallsign(),
            flightPlan.GetOrigin(),
            flightPlan.GetDestination(),
            extractedRoute,
            flightPlan.GetCruiseLevel());

        this->codeCache.RegisterAircraft(flightPlan.GetCallsign(), data);
    }

    /*
        Respond to flightplans disconnecting - invalidate the intention code cache.
    */
    void IntentionCodeEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
        this->codeCache.UnregisterAircraft(flightPlan.GetCallsign());
    }

    /*
        Returns the description of the TagItem.
    */
    auto IntentionCodeEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "UKCP Intention Code";
    }

    /*
        Returns the intention code.
    */
    void IntentionCodeEventHandler::SetTagItemData(TagData& tagData)
    {
        // If we have it cached, then use the cached value
        const auto& flightplan = tagData.GetFlightplan();
        auto extractedRoute = flightplan.GetExtractedRoute();
        if (this->codeCache.HasIntentionCodeForAircraft(flightplan.GetCallsign())) {
            tagData.SetItemString(this->codeCache.GetIntentionCodeForAircraft(flightplan.GetCallsign()));
            return;
        }

        // Generate the code and then cache it
        IntentionCodeData data = this->intention->GetIntentionCodeForFlightplan(
            flightplan.GetCallsign(),
            flightplan.GetOrigin(),
            flightplan.GetDestination(),
            extractedRoute,
            flightplan.GetCruiseLevel());

        this->codeCache.RegisterAircraft(flightplan.GetCallsign(), data);
        tagData.SetItemString(data.intentionCode);
        outboundEvent.SendEvent(std::make_shared<IntentionCodeUpdatedMessage>(
            flightplan.GetCallsign(), data.exitPoint, data.intentionCode));
    }

    /*
        Clear the code cache if the user logs on with a different controller position.
    */
    void IntentionCodeEventHandler::ControllerUpdateEvent(EuroScopeCControllerInterface& controller)
    {
        if (controller.IsCurrentUser() && controller.GetCallsign() != this->intention->GetUserControllerPosition()) {
            this->intention->SetUserControllerPosition(controller.GetCallsign());
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
    void IntentionCodeEventHandler::SelfDisconnectEvent()
    {
        this->codeCache.Clear();
    }

    auto IntentionCodeEventHandler::GetGenerator() const -> const IntentionCodeGenerator&
    {
        return *this->intention;
    }

    auto IntentionCodeEventHandler::GetCache() const -> const IntentionCodeCache&
    {
        return this->codeCache;
    }
} // namespace UKControllerPlugin::IntentionCode
