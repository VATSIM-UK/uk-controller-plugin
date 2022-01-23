#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Aircraft {
    class AircraftType;

    class AircraftTypeMapperInterface
    {
        public:
        virtual ~AircraftTypeMapperInterface() = default;
        [[nodiscard]] virtual auto MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<AircraftType> = 0;
    };
} // namespace UKControllerPlugin::Aircraft
