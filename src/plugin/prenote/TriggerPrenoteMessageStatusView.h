#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageStatusView;

    class TriggerPrenoteMessageStatusView
    {
        public:
        static void Trigger(Euroscope::EuroScopeCFlightPlanInterface& flightplan, POINT mousePos);

        private:
        static inline const std::chrono::seconds DISPLAY_DURATION = std::chrono::seconds(3);
    };
} // namespace UKControllerPlugin::Prenote
