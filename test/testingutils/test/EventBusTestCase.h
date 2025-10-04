#pragma once
#include "eventhandler/DrainableEuroscopeThreadEventSink.h"
#include "eventhandler/EuroscopeThreadEventSink.h"
#include "eventhandler/EventObserver.h"
#include "eventhandler/EventBusFactory.h"
#include "eventhandler/MutableEventBus.h"
#include <typeinfo>
#include <any>

namespace UKControllerPluginUtilsTest {
    class TestEventBusObserver : public UKControllerPluginUtils::EventHandler::EventObserver
    {
        public:
        void OnEvent(std::any event) override
        {
            observedEvents.push_back(event);
        }

        std::vector<std::any> observedEvents{};
    };

    class TestEuroscopeThreadEventProcessor
        : public UKControllerPluginUtils::EventHandler::EuroscopeThreadEventSink,
          public UKControllerPluginUtils::EventHandler::DrainableEuroscopeThreadEventSink
    {
        public:
        void OnEvent(const std::function<void()>& event) override
        {
            event();
        }

        void Drain() override
        {
            // No-op
        }

        std::vector<std::any> processedEvents{};
    };

    class TestEventBusFactory : public UKControllerPluginUtils::EventHandler::EventBusFactory
    {
        public:
        TestEventBusFactory()
            : processor(std::make_shared<TestEuroscopeThreadEventProcessor>()),
              observer(std::make_shared<TestEventBusObserver>())
        {
        }

        auto CreateBus() -> std::unique_ptr<UKControllerPluginUtils::EventHandler::EventBus> override
        {
            auto bus = std::unique_ptr<UKControllerPluginUtils::EventHandler::MutableEventBus>(
                new UKControllerPluginUtils::EventHandler::MutableEventBus(processor));
            bus->SetObserver(observer);

            return std::move(bus);
        }

        auto DrainableEventSink()
            -> std::shared_ptr<UKControllerPluginUtils::EventHandler::DrainableEuroscopeThreadEventSink> override
        {
            return processor;
        }

        std::shared_ptr<TestEuroscopeThreadEventProcessor> processor;
        std::shared_ptr<TestEventBusObserver> observer;
    };

    class EventBusTestCase : public virtual testing::Test
    {
        public:
        virtual void SetUp()
        {
            testing::Test::SetUp();
            busFactory = std::make_shared<TestEventBusFactory>();
            UKControllerPluginUtils::EventHandler::MutableEventBus::MutableEventBus::SetFactory(busFactory);
        }

        virtual void TearDown()
        {
            UKControllerPluginUtils::EventHandler::MutableEventBus::MutableEventBus::Reset();
            testing::Test::TearDown();
        }

        template <typename T>
        void AssertEventDispatched(int eventNumber, const std::function<void(const T&)>& assertion)
        {
            const auto event = std::any_cast<const T>(EventBusObserver().observedEvents[eventNumber]);
            assertion(event);
        }

        template <typename T> void AssertFirstEventDispatched(const std::function<void(const T&)>& assertion)
        {
            AssertEventDispatched<T>(0, assertion);
        }

        void AssertEventDispatchCount(int count)
        {
            EXPECT_EQ(count, EventBusObserver().observedEvents.size());
        }

        void AssertSingleEventDispatched()
        {
            AssertEventDispatchCount(1);
        }

        void AssertNoEventsDispatched()
        {
            AssertEventDispatchCount(0);
        }

        template <typename T>
        auto GetStreamForEventType() const
            -> const std::shared_ptr<UKControllerPluginUtils::EventHandler::EventStream<T>>
        {
            return std::any_cast<std::shared_ptr<UKControllerPluginUtils::EventHandler::EventStream<T>>>(
                UKControllerPluginUtils::EventHandler::EventBus::Bus().GetAnyStream(typeid(T)));
        }

        template <typename T> void AssertEventHandlerRegistrationsCountForEvent(int count)
        {
            EXPECT_EQ(count, GetStreamForEventType<T>()->Handlers().size());
        }

        template <typename T> void AssertSingleEventHandlerRegistrationForEvent()
        {
            AssertEventHandlerRegistrationsCountForEvent<T>(1);
        }

        template <typename HandlerType, typename EventType>
        void AssertHandlerRegisteredForEvent(UKControllerPluginUtils::EventHandler::EventHandlerFlags flags)
        {
            const auto eventStream = GetStreamForEventType<EventType>();
            for (const auto& handler : eventStream->Handlers()) {
                try {
                    static_cast<void>(dynamic_cast<const HandlerType&>(*handler.handler.get()));
                    EXPECT_EQ(flags, handler.flags);
                    return;
                } catch (std::bad_cast&) {
                    // Noop
                }
            }

            FAIL();
        }

        // For observing and making assertions on events
        [[nodiscard]] auto EventBusObserver() const -> TestEventBusObserver&
        {
            return *busFactory->observer;
        }

        private:
        std::shared_ptr<TestEventBusFactory> busFactory;
    };
} // namespace UKControllerPluginUtilsTest
