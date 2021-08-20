#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "prenote/AbstractPrenote.h"

namespace UKControllerPlugin::Euroscope {
    class UserSetting;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Prenote {

    class PrenoteService;

    /*
        Handles events pertaining to prenotes.
    */
    class PrenoteEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        PrenoteEventHandler(
            std::unique_ptr<UKControllerPlugin::Prenote::PrenoteService> prenoteService,
            UKControllerPlugin::Euroscope::UserSetting& userSetting);

        // Inherited via FlightPlanEventHandlerInterface
        void FlightPlanEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void
        FlightPlanDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        [[nodiscard]] auto PrenotesEnabled() const -> bool;

        private:
        // Handles prenoting
        std::unique_ptr<UKControllerPlugin::Prenote::PrenoteService> prenoteService;

        // Place for saving user settings
        UKControllerPlugin::Euroscope::UserSetting& userSetting;
    };
} // namespace UKControllerPlugin::Prenote
