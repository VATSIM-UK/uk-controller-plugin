#include "pch/pch.h"
#include "handoff/HandoffEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPosition.h"
#include "handoff/HandoffFrequencyUpdatedMessage.h"
#include "integration/OutboundIntegrationEventHandler.h"

using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Integration::OutboundIntegrationEventHandler;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin {
    namespace Handoff {

        HandoffEventHandler::HandoffEventHandler(
            const HandoffCollection& handoffs,
            const ActiveCallsignCollection& callsigns,
            OutboundIntegrationEventHandler& outboundEvent
        ) : handoffs(std::move(handoffs)), callsigns(std::move(callsigns)), outboundEvent(outboundEvent)
        {

        }

        /*
            Add an item to the cache.
        */
        void HandoffEventHandler::AddCachedItem(std::string callsign, CachedHandoff item)
        {
            this->cache[callsign] = item;
        }

        size_t HandoffEventHandler::CountCachedItems(void) const
        {
            return this->cache.size();
        }

        /*
            Get the cached item
        */
        CachedHandoff HandoffEventHandler::GetCachedItem(std::string callsign) const
        {
            return this->cache.count(callsign) ? this->cache.at(callsign) : this->DEFAULT_TAG_VALUE;
        }

        std::string HandoffEventHandler::GetTagItemDescription(int tagItemId) const
        {
            return "Departure Handoff Next Controller";
        }

        void HandoffEventHandler::SetTagItemData(TagData & tagData)
        {
            if (this->cache.count(tagData.flightPlan.GetCallsign())) {
                tagData.SetItemString(this->cache[tagData.flightPlan.GetCallsign()].frequency);
                return;
            }

            ControllerPositionHierarchy controllers = this->handoffs.GetSidHandoffOrder(
                tagData.flightPlan.GetOrigin(),
                tagData.flightPlan.GetSidName()
            );

            if (controllers == this->handoffs.invalidHierarchy) {
                this->cache[tagData.flightPlan.GetCallsign()] = this->DEFAULT_TAG_VALUE;
                tagData.SetItemString(this->DEFAULT_TAG_VALUE.frequency);
                return;
            }

            for (
                ControllerPositionHierarchy::const_iterator it = controllers.cbegin();
                it != controllers.cend();
                ++it
            ){

                if (this->callsigns.PositionActive(it->get().GetCallsign())) {
                    // If we're handing off to the user, then don't bother displaying a handoff frequency
                    if (
                        this->callsigns.UserHasCallsign() &&
                        this->callsigns.GetUserCallsign().GetNormalisedPosition() == *it
                    ) {
                        this->cache[tagData.flightPlan.GetCallsign()] = this->DEFAULT_TAG_VALUE;
                        tagData.SetItemString(this->cache[tagData.flightPlan.GetCallsign()].frequency);
                        return;
                    }

                    char frequencyString[24];
                    sprintf_s(frequencyString, "%.3f", it->get().GetFrequency());
                    this->cache[tagData.flightPlan.GetCallsign()] =
                        CachedHandoff(frequencyString, it->get().GetCallsign());
                    tagData.SetItemString(this->cache[tagData.flightPlan.GetCallsign()].frequency);
                    this->FireHandoffUpdatedEvent(tagData.flightPlan.GetCallsign());
                    return;
                }
            }

            this->cache[tagData.flightPlan.GetCallsign()] = this->UNICOM_TAG_VALUE;
            tagData.SetItemString(this->cache[tagData.flightPlan.GetCallsign()].frequency);
            this->FireHandoffUpdatedEvent(tagData.flightPlan.GetCallsign());
        }

        void HandoffEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface& flightPlan,
            EuroScopeCRadarTargetInterface& radarTarget
        ) {
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
        void HandoffEventHandler::ActiveCallsignAdded(const ActiveCallsign& callsign, bool userCallsign)
        {
            this->cache.clear();
        }

        /*
            If a callsign is removed, clear the cache for anything they were involved in.
        */
        void HandoffEventHandler::ActiveCallsignRemoved(const ActiveCallsign& callsign, bool userCallsign)
        {
            for (
                std::map<std::string, CachedHandoff>::const_iterator it = this->cache.cbegin();
                it != this->cache.cend();
            ) {
                auto keyToRemove = it++;
                if (keyToRemove->second.callsign == callsign.GetCallsign()) {
                    this->cache.erase(keyToRemove);
                }
            }
        }

        /*
            If the callsigns are flushed, no cached values are valid
        */
        void HandoffEventHandler::CallsignsFlushed(void)
        {
            this->cache.clear();
        }

        void HandoffEventHandler::FireHandoffUpdatedEvent(std::string callsign)
        {
            this->outboundEvent.SendEvent(
                std::make_shared<HandoffFrequencyUpdatedMessage>(
                    callsign,
                    this->cache[callsign].frequency
                )
            );
        }
    }  // namespace Handoff
}  // namespace UKControllerPlugin
