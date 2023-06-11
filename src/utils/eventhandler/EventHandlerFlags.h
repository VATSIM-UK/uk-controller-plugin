#pragma once

namespace UKControllerPluginUtils::EventHandler {
    // A set of flags for event handlers
    enum class EventHandlerFlags : unsigned int
    {
        Sync = 1 << 0,
        Async = 1 << 1,
        EuroscopeThread = 1 << 2,
    };

    DEFINE_ENUM_FLAG_OPERATORS(EventHandlerFlags)
} // namespace UKControllerPluginUtils::EventHandler
