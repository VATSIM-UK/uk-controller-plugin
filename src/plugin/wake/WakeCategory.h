#pragma once

namespace UKControllerPlugin::Wake {
    struct ArrivalWakeInterval;
    struct DepartureWakeInterval;

    /**
     * Represents a wake category within a given wake scheme
     */
    class WakeCategory
    {
        public:
        WakeCategory(
            int id,
            std::string code,
            std::string description,
            int relativeWeighting,
            std::list<std::shared_ptr<DepartureWakeInterval>> subsequentDepartureIntervals,
            std::list<std::shared_ptr<ArrivalWakeInterval>> subsequentArrivalIntervals);

        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto Code() const -> const std::string&;
        [[nodiscard]] auto Description() const -> const std::string&;
        [[nodiscard]] auto RelativeWeighting() const -> int;
        [[nodiscard]] auto SubsequentDepartureIntervals() const
            -> const std::list<std::shared_ptr<DepartureWakeInterval>>&;
        [[nodiscard]] auto DepartureInterval(const WakeCategory& nextAircraftCategory, bool intermediate) const
            -> std::shared_ptr<DepartureWakeInterval>;
        [[nodiscard]] auto SubsequentArrivalIntervals() const -> const std::list<std::shared_ptr<ArrivalWakeInterval>>&;
        [[nodiscard]] auto ArrivalInterval(const WakeCategory& nextAircraftCategory) const
            -> std::shared_ptr<ArrivalWakeInterval>;

        private:
        // The id in the API
        int id;

        // The code, e.g. LM
        std::string code;

        // Description
        std::string description;

        // The weighting of the category relative to others in the scheme
        int relativeWeighting;

        // Subsequent departure intervals for wake
        std::list<std::shared_ptr<DepartureWakeInterval>> subsequentDepartureIntervals;

        // Subsequent departure intervals for wake
        std::list<std::shared_ptr<ArrivalWakeInterval>> subsequentArrivalIntervals;
    };
} // namespace UKControllerPlugin::Wake
