#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "departure/UserShouldClearDepartureDataEvent.h"
#include "departure/UserShouldClearDepartureDataMonitor.h"
#include "handoff/ResolvedHandoff.h"
#include "mock/MockDepartureHandoffResolver.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "test/EventBusTestCase.h"
#include <gmock/gmock-nice-strict.h>

namespace UKControllerPluginTest::Departure {

    class UserShouldClearDepartureDataMonitorTest : public UKControllerPluginUtilsTest::EventBusTestCase
    {
        public:
        UserShouldClearDepartureDataMonitorTest()
            : position(1, "EGKK_TWR", 118.5, {"EGLL"}, true, false),
              callsign(std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>(
                  "EGKK_TWR", "Test", position, false)),
              userCallsign(
                  std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>("EGKK_TWR", "Test", position, true)),
              handoffResolver(std::make_shared<testing::NiceMock<Handoff::MockDepartureHandoffResolver>>()),
              ownership(std::make_shared<UKControllerPlugin::Ownership::AirfieldServiceProviderCollection>()),
              monitor(handoffResolver, ownership, plugin)
        {
            mockFlightplan = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            ON_CALL(*mockFlightplan, GetCallsign()).WillByDefault(testing::Return("BAW123"));
            ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(mockFlightplan));

            auto handoff = std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(
                "BAW123",
                std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                    -1, "UNICOM", 122.800, std::vector<std::string>{}, true, false),
                nullptr,
                nullptr);
            ON_CALL(*handoffResolver, Resolve(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(handoff));
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan;
        UKControllerPlugin::Controller::ControllerPosition position;
        std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsign> callsign;
        std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsign> userCallsign;
        std::shared_ptr<testing::NiceMock<Handoff::MockDepartureHandoffResolver>> handoffResolver;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
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

        AssertSingleEventDispatched();
        AssertFirstEventDispatched<UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>(
            [](const auto& event) { EXPECT_EQ("BAW123", event.callsign); });
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

        AssertSingleEventDispatched();
        AssertFirstEventDispatched<UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>(
            [](const auto& event) { EXPECT_EQ("BAW123", event.callsign); });
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItFiresEventIfUserIsOnDeliveryWithNoGroundOrTower)
    {
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Delivery, userCallsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        AssertSingleEventDispatched();
        AssertFirstEventDispatched<UKControllerPlugin::Departure::UserShouldClearDepartureDataEvent>(
            [](const auto& event) { EXPECT_EQ("BAW123", event.callsign); });
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

        AssertNoEventsDispatched();
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

        AssertNoEventsDispatched();
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

        AssertNoEventsDispatched();
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

        AssertNoEventsDispatched();
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItDoesntFireEventIfTheresNoHandoffPresent)
    {
        ON_CALL(*handoffResolver, Resolve(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(nullptr));
        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Tower, userCallsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        AssertNoEventsDispatched();
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItDoesntFireEventIfTheHandoffIsNotUnicom)
    {

        auto handoff = std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(
            "BAW123",
            std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                1, "LON_S_CTR", 129.420, std::vector<std::string>{}, true, false),
            nullptr,
            nullptr);
        ON_CALL(*handoffResolver, Resolve(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(handoff));

        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Tower, userCallsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        AssertNoEventsDispatched();
    }

    TEST_F(UserShouldClearDepartureDataMonitorTest, ItDoesntFireEventIfNoFlightplanForcallsign)
    {
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));

        std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(
            UKControllerPlugin::Ownership::ServiceType::Tower, userCallsign));
        ownership->SetProvidersForAirfield("EGKK", provisions);

        monitor.OnEvent({"BAW123", "EGKK"});

        AssertNoEventsDispatched();
    }
} // namespace UKControllerPluginTest::Departure
