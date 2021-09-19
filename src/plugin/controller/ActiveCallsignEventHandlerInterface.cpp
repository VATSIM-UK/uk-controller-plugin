#include "ActiveCallsignEventHandlerInterface.h"

namespace UKControllerPlugin::Controller {
    ActiveCallsignEventHandlerInterface::ActiveCallsignEventHandlerInterface() = default;
    ActiveCallsignEventHandlerInterface::~ActiveCallsignEventHandlerInterface() = default;
    ActiveCallsignEventHandlerInterface::ActiveCallsignEventHandlerInterface(
        const ActiveCallsignEventHandlerInterface&) = default;
    ActiveCallsignEventHandlerInterface::ActiveCallsignEventHandlerInterface(
        ActiveCallsignEventHandlerInterface&&) noexcept = default;
    [[nodiscard]] auto ActiveCallsignEventHandlerInterface::operator=(const ActiveCallsignEventHandlerInterface&)
        -> ActiveCallsignEventHandlerInterface& = default;
    [[nodiscard]] auto ActiveCallsignEventHandlerInterface::operator=(ActiveCallsignEventHandlerInterface&&) noexcept
        -> ActiveCallsignEventHandlerInterface& = default;

    void ActiveCallsignEventHandlerInterface::CallsignsFlushed()
    {
    }
} // namespace UKControllerPlugin::Controller
