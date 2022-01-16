#include "WakeCategory.h"

namespace UKControllerPlugin::Wake {

    WakeCategory::WakeCategory(
        int id,
        std::string code,
        std::string description,
        int relativeWeighting,
        std::list<std::shared_ptr<DepartureWakeInterval>> subsequentDepartureIntervals)
        : id(id), code(std::move(code)), description(std::move(description)), relativeWeighting(relativeWeighting),
          subsequentDepartureIntervals(std::move(subsequentDepartureIntervals))
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
} // namespace UKControllerPlugin::Wake
