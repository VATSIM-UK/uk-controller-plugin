#include "DepartureHandoffResolver.h"
#include "HandoffCache.h"
#include "HandoffEventHandler.h"
#include "HandoffFrequencyUpdatedMessage.h"
#include "ResolvedHandoff.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "tag/TagData.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Integration::OutboundIntegrationEventHandler;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin::Handoff {

    HandoffEventHandler::HandoffEventHandler(
        std::shared_ptr<DepartureHandoffResolver> resolver,
        std::shared_ptr<HandoffCache> cache,
        Integration::OutboundIntegrationEventHandler& outboundEvent)
        : resolver(std::move(resolver)), cache(std::move(cache)), outboundEvent(outboundEvent)
    {
    }

    auto HandoffEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "Departure Handoff Next Controller";
    }

    void HandoffEventHandler::SetTagItemData(TagData& tagData)
    {
        // Try to get the cached item
        const auto& flightplan = tagData.GetFlightplan();
        const auto cachedItem = this->cache->Get(flightplan.GetCallsign());
        if (cachedItem) {
            tagData.SetItemString(FormatFrequency(cachedItem));
            return;
        }

        // Resolve the handoff and return
        const auto resolvedHandoff = this->resolver->Resolve(flightplan);
        this->cache->Add(resolvedHandoff);
        tagData.SetItemString(FormatFrequency(resolvedHandoff));
        this->FireHandoffUpdatedEvent(flightplan.GetCallsign());
    }

    void HandoffEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
    {
        this->cache->Delete(flightPlan.GetCallsign());
    }

    void HandoffEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
        this->cache->Delete(flightPlan.GetCallsign());
    }

    void HandoffEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        // No change required here.
    }

    void HandoffEventHandler::FireHandoffUpdatedEvent(const std::string& callsign)
    {
        this->outboundEvent.SendEvent(
            std::make_shared<HandoffFrequencyUpdatedMessage>(callsign, FormatFrequency(this->cache->Get(callsign))));
    }

    auto HandoffEventHandler::GetCache() const -> std::shared_ptr<HandoffCache>
    {
        return this->cache;
    }

    auto HandoffEventHandler::FormatFrequency(const std::shared_ptr<ResolvedHandoff>& handoff) -> std::string
    {
        char frequencyString[FREQUENCY_BUFFER_LENGTH];                                   // NOLINT
        sprintf_s(frequencyString, "%.3f", handoff->resolvedController->GetFrequency()); // NOLINT
        return frequencyString;                                                          // NOLINT
    }
} // namespace UKControllerPlugin::Handoff
