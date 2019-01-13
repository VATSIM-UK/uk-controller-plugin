#pragma once

namespace UKControllerPlugin {
    namespace TimedEvent {
        class TimedEventCollection;
    }  // namespace TimedEvent
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace TimedEvent {
        void DeferredEventBootstrap(UKControllerPlugin::TimedEvent::TimedEventCollection & timedEvents);
    }  // namespace TimedEvent
}  // namespace UKControllerPlugin
