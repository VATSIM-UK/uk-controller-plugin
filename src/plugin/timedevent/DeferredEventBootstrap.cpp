#include "pch/stdafx.h"
#include "timedevent/DeferredEventBootstrap.h"
#include "timedevent/DeferredEventHandler.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::TimedEvent::DeferredEventHandler;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPlugin {
    namespace TimedEvent {

        void DeferredEventBootstrap(TimedEventCollection& timedEvents)
        {
            timedEvents.RegisterEvent(std::make_shared<DeferredEventHandler>(), 5);
        }

    } // namespace TimedEvent
} // namespace  UKControllerPlugin
