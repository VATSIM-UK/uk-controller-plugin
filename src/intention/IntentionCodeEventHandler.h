#pragma once
#include "tag/TagItemInterface.h"
#include "controller/ControllerStatusEventHandlerInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "intention/IntentionCodeGenerator.h"
#include "intention/IntentionCodeCache.h"
#include "tag/TagData.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace IntentionCode {
        class IntentionCode;
        class IntentionCodeCache;
    }  // namespace IntentionCode
    namespace Integration {
        class OutboundIntegrationEventHandler;
    } // namespace Integration
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            A class for generating intention code tag items.
        */
        class IntentionCodeEventHandler
            : public Tag::TagItemInterface,
              public Flightplan::FlightPlanEventHandlerInterface,
              public Controller::ControllerStatusEventHandlerInterface
        {
            public:
                IntentionCodeEventHandler(
                    IntentionCodeGenerator intention,
                    IntentionCodeCache codeCache,
                    Integration::OutboundIntegrationEventHandler& outboundEvent
                );
                void ControllerFlightPlanDataEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    int dataType
                );
                void FlightPlanEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget
                );
                void FlightPlanDisconnectEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan
                );
                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(Tag::TagData& tagData) override;


                // Inherited via ControllerStatusEventHandlerInterface
                void ControllerUpdateEvent(
                    Euroscope::EuroScopeCControllerInterface& controller
                ) override;
                void ControllerDisconnectEvent(
                    Euroscope::EuroScopeCControllerInterface& controller
                ) override;
                void SelfDisconnectEvent(void) override;

                const IntentionCodeGenerator& GetGenerator() const;
                const IntentionCodeCache& GetCache() const;

            private:

                // A class for generating intention codes
                IntentionCodeGenerator intention;

                // A cache for codes that have already been generated
                IntentionCodeCache codeCache;

                // Allows us to send outbound events
                Integration::OutboundIntegrationEventHandler& outboundEvent;
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
