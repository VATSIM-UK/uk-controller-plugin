#pragma once

#include "WakeCategoryFactory.h"

namespace UKControllerPlugin::Wake {
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
            std::list<std::shared_ptr<DepartureWakeInterval>> subsequentDepartureIntervals);

        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto Code() const -> const std::string&;
        [[nodiscard]] auto Description() const -> const std::string&;
        [[nodiscard]] auto RelativeWeighting() const -> int;
        [[nodiscard]] auto SubsequentDepartureIntervals() const
            -> const std::list<std::shared_ptr<DepartureWakeInterval>>&;

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
    };
} // namespace UKControllerPlugin::Wake
