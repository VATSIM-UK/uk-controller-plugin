#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
    class EuroScopeCRadarTargetInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Squawk {

    /*
        Makes the relevant API calls to generate a squawk for an aircraft.
    */
    class SquawkGeneratorInterface
    {
        public:
        virtual ~SquawkGeneratorInterface() = default;
        virtual auto DeleteApiSquawkAndSetTo(
            const std::string& squawk, UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> bool = 0;
        virtual auto ForceGeneralSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool = 0;
        virtual auto ForceLocalSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool = 0;
    };
} // namespace UKControllerPlugin::Squawk
