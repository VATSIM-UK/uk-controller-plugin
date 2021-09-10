#include "AbstractTimedEvent.h"

namespace UKControllerPlugin::TimedEvent {
    AbstractTimedEvent::AbstractTimedEvent() = default;
    AbstractTimedEvent::~AbstractTimedEvent() = default;
    AbstractTimedEvent::AbstractTimedEvent(const AbstractTimedEvent&) = default;
    AbstractTimedEvent::AbstractTimedEvent(AbstractTimedEvent&&) noexcept = default;
    auto AbstractTimedEvent::operator=(const AbstractTimedEvent&) -> AbstractTimedEvent& = default;
    auto AbstractTimedEvent::operator=(AbstractTimedEvent&&) noexcept -> AbstractTimedEvent& = default;
} // namespace UKControllerPlugin::TimedEvent
