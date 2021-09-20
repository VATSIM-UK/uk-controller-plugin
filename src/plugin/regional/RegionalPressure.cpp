#include "RegionalPressure.h"

namespace UKControllerPlugin::Regional {

    auto RegionalPressure::IsAcknowledged() const -> bool
    {
        return this->acknowledgedAt > this->updatedAt;
    }

    void RegionalPressure::Acknowledge()
    {
        this->acknowledgedAt = std::chrono::system_clock::now();
    }

    auto RegionalPressure::operator==(const RegionalPressure& compare) const -> bool
    {
        return this->name == compare.name && this->pressure == compare.pressure &&
               this->acknowledgedAt == compare.acknowledgedAt;
    }
} // namespace UKControllerPlugin::Regional
