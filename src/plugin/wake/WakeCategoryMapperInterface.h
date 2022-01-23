#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Wake {
    class WakeCategory;

    class WakeCategoryMapperInterface
    {
        public:
        virtual ~WakeCategoryMapperInterface() = default;
        [[nodiscard]] virtual auto MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> std::shared_ptr<WakeCategory> = 0;
    };
} // namespace UKControllerPlugin::Wake
