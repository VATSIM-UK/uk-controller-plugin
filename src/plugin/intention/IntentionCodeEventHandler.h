#pragma once
#include "controller/ControllerStatusEventHandlerInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "intention/IntentionCodeCache.h"
#include "intention/IntentionCodeGenerator.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace IntentionCode {
        class IntentionCode;
        class IntentionCodeCache;
    } // namespace IntentionCode
    namespace Integration {
        class OutboundIntegrationEventHandler;
    } // namespace Integration
} // namespace UKControllerPlugin

namespace UKControllerPlugin::IntentionCode {

    /*
        A class for generating intention code tag items.
    */
    class IntentionCodeEventHandler : public Tag::TagItemInterface,
                                      public Flightplan::FlightPlanEventHandlerInterface,
                                      public Controller::ControllerStatusEventHandlerInterface
    {
        public:
        IntentionCodeEventHandler(
            IntentionCodeGenerator intention,
            IntentionCodeCache codeCache,
            Integration::OutboundIntegrationEventHandler& outboundEvent);
        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        // Inherited via ControllerStatusEventHandlerInterface
        void ControllerUpdateEvent(Euroscope::EuroScopeCControllerInterface& controller) override;
        void ControllerDisconnectEvent(Euroscope::EuroScopeCControllerInterface& controller) override;
        void SelfDisconnectEvent() override;

        [[nodiscard]] auto GetGenerator() const -> const IntentionCodeGenerator&;
        [[nodiscard]] auto GetCache() const -> const IntentionCodeCache&;

        private:
        // A class for generating intention codes
        IntentionCodeGenerator intention;

        // A cache for codes that have already been generated
        IntentionCodeCache codeCache;

        // Allows us to send outbound events
        Integration::OutboundIntegrationEventHandler& outboundEvent;
    };
} // namespace UKControllerPlugin::IntentionCode
