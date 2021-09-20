#include "MetarEventHandlerInterface.h"

namespace UKControllerPlugin::Metar {
    MetarEventHandlerInterface::MetarEventHandlerInterface() = default;
    MetarEventHandlerInterface::~MetarEventHandlerInterface() = default;
    MetarEventHandlerInterface::MetarEventHandlerInterface(const MetarEventHandlerInterface&) = default;
    MetarEventHandlerInterface::MetarEventHandlerInterface(MetarEventHandlerInterface&&) noexcept = default;
    [[nodiscard]] auto MetarEventHandlerInterface::operator=(const MetarEventHandlerInterface&)
        -> MetarEventHandlerInterface& = default;
    [[nodiscard]] auto MetarEventHandlerInterface::operator=(MetarEventHandlerInterface&&) noexcept
        -> MetarEventHandlerInterface& = default;
} // namespace UKControllerPlugin::Metar
