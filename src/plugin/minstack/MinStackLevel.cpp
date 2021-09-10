#include "MinStackLevel.h"

namespace UKControllerPlugin::MinStack {
    auto MinStackLevel::IsAcknowledged() const -> bool
    {
        return this->acknowledgedAt > this->updatedAt;
    }

    void MinStackLevel::Acknowledge()
    {
        this->acknowledgedAt = std::chrono::system_clock::now();
    }

    auto MinStackLevel::operator==(const MinStackLevel& compare) const -> bool
    {
        return this->type == compare.type && this->name == compare.name && this->msl == compare.msl &&
               this->acknowledgedAt == compare.acknowledgedAt;
    }
} // namespace UKControllerPlugin::MinStack
