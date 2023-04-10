#pragma once

namespace UKControllerPlugin::EventHandler {
    // A set of flags for event handlers
    enum class EventHandlerFlags : unsigned int
    {
        Sync = 1 << 0,
        Async = 1 << 1
    };

    DEFINE_ENUM_FLAG_OPERATORS(EventHandlerFlags)
} // namespace UKControllerPlugin::EventHandler
