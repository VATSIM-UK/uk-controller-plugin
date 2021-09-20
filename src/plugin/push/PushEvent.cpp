#include "PushEvent.h"

namespace UKControllerPlugin::Push {

    [[nodiscard]] auto PushEvent::operator==(const PushEvent& compare) const -> bool
    {
        return this->channel == compare.channel && this->event == compare.event && this->data == compare.data;
    }
} // namespace UKControllerPlugin::Push
