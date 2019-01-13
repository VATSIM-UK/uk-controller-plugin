#pragma once
#include "pch/pch.h"
#include "timedevent/DeferredEventBootstrap.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::TimedEvent::DeferredEventBootstrap;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest {
    namespace TimedEvent {

        TEST(DeferredEventBootstrapTest, ItRegistersForTimedEvents)
        {
            TimedEventCollection collection;
            EXPECT_EQ(0, collection.CountHandlers());
            DeferredEventBootstrap(collection);
            EXPECT_EQ(1, collection.CountHandlers());
            EXPECT_EQ(1, collection.CountHandlersForFrequency(5));
        }

    }  // namespace TimedEvent
}  // namespace UKControllerPluginTest
