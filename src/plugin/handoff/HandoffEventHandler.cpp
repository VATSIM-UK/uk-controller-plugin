#include "HandoffEventHandler.h"
#include "HandoffFrequencyUpdatedMessage.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Integration::OutboundIntegrationEventHandler;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin::Handoff {

    HandoffEventHandler::HandoffEventHandler(
        const HandoffCollection& handoffs,
        const ActiveCallsignCollection& callsigns,
        OutboundIntegrationEventHandler& outboundEvent)
        : handoffs(handoffs), callsigns(callsigns), outboundEvent(outboundEvent)
    {
    }

    /*
        Add an item to the cache.
    */
    void HandoffEventHandler::AddCachedItem(const std::string& callsign, CachedHandoff item)
    {
        this->cache[callsign] = std::move(item);
    }

    /*
        Get the cached item
    */
    auto HandoffEventHandler::GetCachedItem(const std::string& callsign) const -> CachedHandoff
    {
        return this->cache.count(callsign) != 0 ? this->cache.at(callsign) : this->DEFAULT_TAG_VALUE;
    }

    auto HandoffEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "Departure Handoff Next Controller";
    }

    void HandoffEventHandler::SetTagItemData(TagData& tagData)
    {
        const auto& flightplan = tagData.GetFlightplan();
        if (this->cache.count(flightplan.GetCallsign()) != 0) {
            tagData.SetItemString(this->cache[flightplan.GetCallsign()].frequency);
            return;
        }

        ControllerPositionHierarchy controllers =
            this->handoffs.GetSidHandoffOrder(flightplan.GetOrigin(), flightplan.GetSidName());

        if (controllers == this->handoffs.invalidHierarchy) {
            this->cache[flightplan.GetCallsign()] = this->DEFAULT_TAG_VALUE;
            tagData.SetItemString(this->DEFAULT_TAG_VALUE.frequency);
            return;
        }

        for (auto it = controllers.cbegin(); it != controllers.cend(); ++it) {

            if (this->callsigns.PositionActive(it->get().GetCallsign())) {
                // If we're handing off to the user, then don't bother displaying a handoff frequency
                if (this->callsigns.UserHasCallsign() &&
                    this->callsigns.GetUserCallsign().GetNormalisedPosition() == *it) {
                    this->cache[flightplan.GetCallsign()] = this->DEFAULT_TAG_VALUE;
                    tagData.SetItemString(this->cache[flightplan.GetCallsign()].frequency);
                    return;
                }

                char frequencyString[24];                                     // NOLINT
                sprintf_s(frequencyString, "%.3f", it->get().GetFrequency()); // NOLINT
                this->cache[flightplan.GetCallsign()] =
                    CachedHandoff(frequencyString, it->get().GetCallsign()); // NOLINT
                tagData.SetItemString(this->cache[flightplan.GetCallsign()].frequency);
                this->FireHandoffUpdatedEvent(flightplan.GetCallsign());
                return;
            }
        }

        this->cache[flightplan.GetCallsign()] = this->UNICOM_TAG_VALUE;
        tagData.SetItemString(this->cache[flightplan.GetCallsign()].frequency);
        this->FireHandoffUpdatedEvent(flightplan.GetCallsign());
    }

    void HandoffEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
    {
        // FP changed, so erase the cache.
        this->cache.erase(flightPlan.GetCallsign());
    }

    void HandoffEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
        // FP gone, so erase the cache.
        this->cache.erase(flightPlan.GetCallsign());
    }

    void HandoffEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        // No change required here.
    }

    /*
        If a new callsign comes along, we have to clear the cache.
    */
    void HandoffEventHandler::ActiveCallsignAdded(const ActiveCallsign& callsign)
    {
        this->cache.clear();
    }

    /*
        If a callsign is removed, clear the cache for anything they were involved in.
    */
    void HandoffEventHandler::ActiveCallsignRemoved(const ActiveCallsign& callsign)
    {
        for (auto it = this->cache.cbegin(); it != this->cache.cend();) {
            auto keyToRemove = it++;
            if (keyToRemove->second.callsign == callsign.GetCallsign()) {
                this->cache.erase(keyToRemove);
            }
        }
    }

    /*
        If the callsigns are flushed, no cached values are valid
    */
    void HandoffEventHandler::CallsignsFlushed()
    {
        this->cache.clear();
    }

    void HandoffEventHandler::FireHandoffUpdatedEvent(const std::string& callsign)
    {
        this->outboundEvent.SendEvent(
            std::make_shared<HandoffFrequencyUpdatedMessage>(callsign, this->cache[callsign].frequency));
    }
} // namespace UKControllerPlugin::Handoff
