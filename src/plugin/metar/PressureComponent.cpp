#include "PressureComponent.h"

namespace UKControllerPlugin::Metar {
    PressureComponent::PressureComponent(
        int qnhHectopascals, int qfeHectopascals, float qnhInHg, float qfeInHg, PressureUnit reportedIn)
        : qnhHectopascals(qnhHectopascals), qfeHectopascals(qfeHectopascals), qnhInHg(qnhInHg), qfeInHg(qfeInHg),
          reportedIn(reportedIn)
    {
    }

    auto PressureComponent::QnhHectopascals() const -> int
    {
        return qnhHectopascals;
    }

    auto PressureComponent::QfeHectopascals() const -> int
    {
        return qfeHectopascals;
    }

    auto PressureComponent::QnhInHg() const -> float
    {
        return qnhInHg;
    }

    auto PressureComponent::QfeInHg() const -> float
    {
        return qfeInHg;
    }

    auto PressureComponent::ReportedIn() const -> PressureUnit
    {
        return reportedIn;
    }

    auto PressureComponent::ReportedAsHectopascals() const -> bool
    {
        return ReportedIn() == PressureUnit::Hectopascals;
    }

    auto PressureComponent::ReportedAsInHg() const -> bool
    {
        return ReportedIn() == PressureUnit::InHg;
    }
} // namespace UKControllerPlugin::Metar
