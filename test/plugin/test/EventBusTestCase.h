#pragma once
#include "eventhandler/EventObserver.h"
#include "eventhandler/EventBusFactory.h"
#include "eventhandler/MutableEventBus.h"

namespace UKControllerPluginTest {
    class TestEventBusObserver : public UKControllerPlugin::EventHandler::EventObserver
    {
        public:
        void OnEvent(std::any event) override
        {
            observedEvents.push_back(event);
        }

        std::vector<std::any> observedEvents{};
    };

    class TestEventBusFactory : public UKControllerPlugin::EventHandler::EventBusFactory
    {
        public:
        TestEventBusFactory() : observer(std::make_shared<TestEventBusObserver>())
        {
        }

        auto CreateBus() -> std::unique_ptr<UKControllerPlugin::EventHandler::EventBus> override
        {
            auto bus = std::unique_ptr<UKControllerPlugin::EventHandler::MutableEventBus>(
                new UKControllerPlugin::EventHandler::MutableEventBus);
            bus->SetObserver(observer);

            return std::move(bus);
        }

        std::shared_ptr<TestEventBusObserver> observer;
    };

    class EventBusTestCase : public testing::Test
    {
        public:
        virtual void SetUp()
        {
            testing::Test::SetUp();
            busFactory = std::make_shared<TestEventBusFactory>();
            UKControllerPlugin::EventHandler::MutableEventBus::MutableEventBus::SetFactory(busFactory);
        }

        virtual void TearDown()
        {
            UKControllerPlugin::EventHandler::MutableEventBus::MutableEventBus::Reset();
            testing::Test::TearDown();
        }

        // For observing and making assertions on events
        [[nodiscard]] auto EventBusObserver() const -> TestEventBusObserver&
        {
            return *busFactory->observer;
        }

        private:
        std::shared_ptr<TestEventBusFactory> busFactory;
    };
} // namespace UKControllerPluginTest
