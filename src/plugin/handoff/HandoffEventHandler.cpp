#include "pch/pch.h"
#include "handoff/HandoffEventHandlerInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPosition.h"
#include "handoff/HandoffFrequencyUpdatedMessage.h"
#include "integration/OutboundIntegrationEventHandler.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Integration::OutboundIntegrationEventHandler;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin {
    namespace Handoff {

        HandoffEventHandlerInterface::HandoffEventHandlerInterface(
            const HandoffCollection& handoffs,
            const ActiveCallsignCollection& callsigns,
            OutboundIntegrationEventHandler& outboundEvent)
            : handoffs(std::move(handoffs)), callsigns(std::move(callsigns)), outboundEvent(outboundEvent)
        {
        }

        /*
            Add an item to the cache.
        */
        void HandoffEventHandlerInterface::AddCachedItem(std::string callsign, CachedHandoff item)
        {
            this->cache[callsign] = item;
        }

        size_t HandoffEventHandlerInterface::CountCachedItems(void) const
        {
            return this->cache.size();
        }

        /*
            Get the cached item
        */
        CachedHandoff HandoffEventHandlerInterface::GetCachedItem(std::string callsign) const
        {
            return this->cache.count(callsign) ? this->cache.at(callsign) : this->DEFAULT_TAG_VALUE;
        }

        std::string HandoffEventHandlerInterface::GetTagItemDescription(int tagItemId) const
        {
            return "Departure Handoff Next Controller";
        }

        void HandoffEventHandlerInterface::SetTagItemData(TagData& tagData)
        {
            const auto& flightplan = tagData.GetFlightplan();
            if (this->cache.count(flightplan.GetCallsign())) {
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

            for (ControllerPositionHierarchy::const_iterator it = controllers.cbegin(); it != controllers.cend();
                 ++it) {

                if (this->callsigns.PositionActive(it->get().GetCallsign())) {
                    // If we're handing off to the user, then don't bother displaying a handoff frequency
                    if (this->callsigns.UserHasCallsign() &&
                        this->callsigns.GetUserCallsign().GetNormalisedPosition() == *it) {
                        this->cache[flightplan.GetCallsign()] = this->DEFAULT_TAG_VALUE;
                        tagData.SetItemString(this->cache[flightplan.GetCallsign()].frequency);
                        return;
                    }

                    char frequencyString[24];
                    sprintf_s(frequencyString, "%.3f", it->get().GetFrequency());
                    this->cache[flightplan.GetCallsign()] = CachedHandoff(frequencyString, it->get().GetCallsign());
                    tagData.SetItemString(this->cache[flightplan.GetCallsign()].frequency);
                    this->FireHandoffUpdatedEvent(flightplan.GetCallsign());
                    return;
                }
            }

            this->cache[flightplan.GetCallsign()] = this->UNICOM_TAG_VALUE;
            tagData.SetItemString(this->cache[flightplan.GetCallsign()].frequency);
            this->FireHandoffUpdatedEvent(flightplan.GetCallsign());
        }

        void HandoffEventHandlerInterface::FlightPlanEvent(
            EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
        {
            // FP changed, so erase the cache.
            this->cache.erase(flightPlan.GetCallsign());
        }

        void HandoffEventHandlerInterface::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
        {
            // FP gone, so erase the cache.
            this->cache.erase(flightPlan.GetCallsign());
        }

        void HandoffEventHandlerInterface::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
        {
            // No change required here.
        }

        /*
            If a new callsign comes along, we have to clear the cache.
        */
        void HandoffEventHandlerInterface::ActiveCallsignAdded(const ActiveCallsign& callsign, bool userCallsign)
        {
            this->cache.clear();
        }

        /*
            If a callsign is removed, clear the cache for anything they were involved in.
        */
        void HandoffEventHandlerInterface::ActiveCallsignRemoved(const ActiveCallsign& callsign, bool userCallsign)
        {
            for (std::map<std::string, CachedHandoff>::const_iterator it = this->cache.cbegin();
                 it != this->cache.cend();) {
                auto keyToRemove = it++;
                if (keyToRemove->second.callsign == callsign.GetCallsign()) {
                    this->cache.erase(keyToRemove);
                }
            }
        }

        /*
            If the callsigns are flushed, no cached values are valid
        */
        void HandoffEventHandlerInterface::CallsignsFlushed(void)
        {
            this->cache.clear();
        }

        void HandoffEventHandlerInterface::FireHandoffUpdatedEvent(std::string callsign)
        {
            this->outboundEvent.SendEvent(
                std::make_shared<HandoffFrequencyUpdatedMessage>(callsign, this->cache[callsign].frequency));
        }
    } // namespace Handoff
} // namespace UKControllerPlugin
