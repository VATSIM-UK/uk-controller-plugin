#include "ArrivalWakeInterval.h"
#include "DepartureWakeInterval.h"
#include "WakeCategory.h"

namespace UKControllerPlugin::Wake {

    WakeCategory::WakeCategory(
        int id,
        std::string code,
        std::string description,
        int relativeWeighting,
        std::list<std::shared_ptr<DepartureWakeInterval>> subsequentDepartureIntervals,
        std::list<std::shared_ptr<ArrivalWakeInterval>> subsequentArrivalIntervals)
        : id(id), code(std::move(code)), description(std::move(description)), relativeWeighting(relativeWeighting),
          subsequentDepartureIntervals(std::move(subsequentDepartureIntervals)),
          subsequentArrivalIntervals(std::move(subsequentArrivalIntervals))
    {
    }

    int WakeCategory::Id() const
    {
        return id;
    }

    auto WakeCategory::Code() const -> const std::string&
    {
        return code;
    }

    auto WakeCategory::Description() const -> const std::string&
    {
        return description;
    }

    auto WakeCategory::RelativeWeighting() const -> int
    {
        return relativeWeighting;
    }

    auto WakeCategory::SubsequentDepartureIntervals() const -> const std::list<std::shared_ptr<DepartureWakeInterval>>&
    {
        return subsequentDepartureIntervals;
    }

    auto WakeCategory::DepartureInterval(const WakeCategory& nextAircraftCategory, bool intermediate) const
        -> std::shared_ptr<DepartureWakeInterval>
    {
        auto matchingInterval = std::find_if(
            subsequentDepartureIntervals.cbegin(),
            subsequentDepartureIntervals.cend(),
            [&nextAircraftCategory, &intermediate](const std::shared_ptr<DepartureWakeInterval>& interval) -> bool {
                return interval->Intermediate() == intermediate &&
                       interval->SubsequentCategory() == nextAircraftCategory.Id();
            });

        return matchingInterval == subsequentDepartureIntervals.cend() ? nullptr : *matchingInterval;
    }

    auto WakeCategory::ArrivalInterval(const WakeCategory& nextAircraftCategory) const
        -> std::shared_ptr<ArrivalWakeInterval>
    {
        auto matchingInterval = std::find_if(
            subsequentArrivalIntervals.cbegin(),
            subsequentArrivalIntervals.cend(),
            [&nextAircraftCategory](const std::shared_ptr<ArrivalWakeInterval>& interval) -> bool {
                return interval->SubsequentCategory() == nextAircraftCategory.Id();
            });

        return matchingInterval == subsequentArrivalIntervals.cend() ? nullptr : *matchingInterval;
    }

    auto WakeCategory::SubsequentArrivalIntervals() const -> const std::list<std::shared_ptr<ArrivalWakeInterval>>&
    {
        return subsequentArrivalIntervals;
    }
} // namespace UKControllerPlugin::Wake
