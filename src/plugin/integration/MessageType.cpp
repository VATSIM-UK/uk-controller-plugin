#include "MessageType.h"

namespace UKControllerPlugin::Integration {
    nlohmann::json MessageType::ToJson() const
    {
        return nlohmann::json::object({{"type", this->type}, {"version", this->version}});
    }

    auto MessageType::operator==(const MessageType& compare) const -> bool
    {
        return this->type == compare.type && this->version == compare.version;
    }

    auto MessageType::operator!=(const MessageType& compare) const -> bool
    {
        return !(*this == compare);
    }

    auto MessageType::operator<(const MessageType& compare) const -> bool
    {
        return this->type < compare.type || this->version < compare.version;
    }
} // namespace UKControllerPlugin::Integration
