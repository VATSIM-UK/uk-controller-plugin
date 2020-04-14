#pragma once
#include "tag/TagItemInterface.h"
#include "controller/ControllerStatusEventHandlerInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "intention/IntentionCodeGenerator.h"
#include "intention/IntentionCodeCache.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace IntentionCode {
        class IntentionCode;
        class IntentionCodeCache;
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            A class for generating intention code tag items.
        */
        class IntentionCodeEventHandler
            : public UKControllerPlugin::Tag::TagItemInterface,
            public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
            public UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface
        {
            public:
                IntentionCodeEventHandler(
                    UKControllerPlugin::IntentionCode::IntentionCodeGenerator intention,
                    UKControllerPlugin::IntentionCode::IntentionCodeCache codeCache
                );
                void ControllerFlightPlanDataEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    int dataType
                );
                void FlightPlanEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                );
                void FlightPlanDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan
                );
                std::string GetTagItemDescription(void) const;
                std::string GetTagItemData(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                );

            private:

                // A class for generating intention codes
                UKControllerPlugin::IntentionCode::IntentionCodeGenerator intention;

                // A cache for codes that have already been generated
                UKControllerPlugin::IntentionCode::IntentionCodeCache codeCache;

                // Inherited via ControllerStatusEventHandlerInterface
                virtual void ControllerUpdateEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& controller) override;
                virtual void ControllerDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& controller) override;
                virtual void SelfDisconnectEvent(void) override;
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
