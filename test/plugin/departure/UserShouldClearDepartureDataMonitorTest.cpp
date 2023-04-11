#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "departure/UserShouldClearDepartureDataEvent.h"
#include "departure/UserShouldClearDepartureDataMonitor.h"
#include "handoff/HandoffCache.h"
#include "handoff/ResolvedHandoff.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "test/EventBusTestCase.h"

namespace UKControllerPluginTest::Departure {

    class UserShouldClearDepartureDataMonitorTest : public EventBusTestCase
    {
        public:
        UserShouldClearDepartureDataMonitorTest()
            : position(1, "EGKK_TWR", 118.5, {"EGLL"}, true, false),
              callsign(std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>(
                  "EGKK_TWR", "Test", position, false)),
              userCallsign(
                  std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>("EGKK_TWR", "Test", position, true)),
              handoffs(std::make_shared<UKControllerPlugin::Handoff::HandoffCache>()),
              ownership(std::make_shared<UKControllerPlugin::Ownership::AirfieldServiceProviderCollection>()),
              monitor(handoffs, ownership)
        {
            handoffs->Add(std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(
                "BAW123",
                std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                    -1, "UNICOM", 122.800, std::vector<std::string>{}, true, false),
                nullptr,
                nullptr));
        }

        UKControllerPlugin::Controller::ControllerPosition position;
        std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsign> callsign;
        std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsign> userCallsign;
        std::shared_ptr<UKControllerPlugin::Handoff::HandoffCache> handoffs;
        std::shared_ptr<UKControllerPlugin::Ownership::AirfieldServiceProviderCollection> ownership;
        UKControllerPlugin::Departure::UserShouldClearDepartureDataMonitor monitor;
    };

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItFiresEventIfUserIsOnTower)
    {
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Tower, userCallsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        EXPECT_EQ(1, EventBusObserver().observedEvents.size());
        EXPECT_EQ(
            "BAW123",
            std::any_cast<UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>(
                EventBusObserver().observedEvents[0])
                .callsign);
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItFiresEventIfUserIsOnGroundWithNoTower)
    {
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Ground, userCallsign));
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Delivery, callsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        EXPECT_EQ(1, EventBusObserver().observedEvents.size());
        EXPECT_EQ(
            "BAW123",
            std::any_cast<UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>(
                EventBusObserver().observedEvents[0])
                .callsign);
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItFiresEventIfUserIsOnDeliveryWithNoGroundOrTower)
    {
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Delivery, userCallsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        EXPECT_EQ(1, EventBusObserver().observedEvents.size());
        EXPECT_EQ(
            "BAW123",
            std::any_cast<UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>(
                EventBusObserver().observedEvents[0])
                .callsign);
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItDoesntFireEventIfUserIsOnDeliveryWithGroundProvidedBySomeoneElse)
    {
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Delivery, userCallsign));
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Ground, callsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItDoesntFireEventIfUserIsOnDeliveryWithTowerProvidedBySomeoneElse)
    {
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Delivery, userCallsign));
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Tower, callsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItDoesntFireEventIfUserIsOnGroundWithTowerProvidedBySomeoneElse)
    {
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Ground, userCallsign));
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Tower, callsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItDoesntFireEventIfUserIsProvidingApproach)
    {
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Approach, userCallsign));
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Tower, userCallsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItDoesntFireEventIfTheresNoHandoffPresent)
    {
        handoffs->Add(
            std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>("BAW123", nullptr, nullptr, nullptr));
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Tower, userCallsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);
        handoffs->Delete("BAW123");

        monitor.OnEvent({"BAW123", "EGKK"});

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItDoesntFireEventIfTheHandoffIsNotUnicom)
    {
        handoffs->Add(
            std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>("BAW123", nullptr, nullptr, nullptr));
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Tower, userCallsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);
        handoffs->Delete("BAW123");
        handoffs->Add(std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(
            "BAW123",
            std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                1, "LON_S_CTR", 129.420, std::vector<std::string>{}, true, false),
            nullptr,
            nullptr));

        monitor.OnEvent({"BAW123", "EGKK"});

        EXPECT_EQ(0, EventBusObserver().observedEvents.size());
    }
} // namespace UKControllerPluginTest::Departure
