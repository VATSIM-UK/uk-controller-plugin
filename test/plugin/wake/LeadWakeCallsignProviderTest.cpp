#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "wake/LeadWakeCallsignProvider.h"
#include "wake/WakeCalculatorOptions.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Wake::LeadWakeCallsignProvider;
using UKControllerPlugin::Wake::WakeCalculatorOptions;

namespace UKControllerPluginTest::Wake {
    class LeadWakeCallsignProviderTest : public testing::Test
    {
        public:
        LeadWakeCallsignProviderTest()
            : position(1, "EGLL_TWR", 118.5, {"EGLL"}, true, false),
              callsign(std::make_shared<ActiveCallsign>("EGKK_TWR", "Test", position, true)),
              options(std::make_shared<WakeCalculatorOptions>()), callsignProvider(serviceProviders, plugin, options)
        {
            flightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            flightplan2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            radarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
            radarTarget2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

            ON_CALL(*flightplan1, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*flightplan1, GetOrigin).WillByDefault(testing::Return("EGLL"));

            ON_CALL(*flightplan2, GetCallsign).WillByDefault(testing::Return("BAW456"));
            ON_CALL(*flightplan2, GetOrigin).WillByDefault(testing::Return("EGLL"));
        }

        ControllerPosition position;
        std::shared_ptr<ActiveCallsign> callsign;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan2;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget2;
        std::shared_ptr<WakeCalculatorOptions> options;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        AirfieldServiceProviderCollection serviceProviders;
        LeadWakeCallsignProvider callsignProvider;
    };

    TEST_F(LeadWakeCallsignProviderTest, ItReturnsRelevantCallsigns)
    {
        serviceProviders.SetProvidersForAirfield(
            "EGLL",
            std::vector<std::shared_ptr<ServiceProvision>>{
                std::make_shared<ServiceProvision>(ServiceType::Tower, callsign)});

        plugin.AddAllFlightplansItem({flightplan1, radarTarget1});
        plugin.AddAllFlightplansItem({flightplan2, radarTarget2});

        EXPECT_EQ(std::set<std::string>({"BAW123", "BAW456"}), callsignProvider.GetCallsigns());
    }

    TEST_F(LeadWakeCallsignProviderTest, ItSkipsCallsignsIfWrongService)
    {
        serviceProviders.SetProvidersForAirfield(
            "EGLL",
            std::vector<std::shared_ptr<ServiceProvision>>{
                std::make_shared<ServiceProvision>(ServiceType::Approach, callsign)});

        plugin.AddAllFlightplansItem({flightplan1, radarTarget1});

        EXPECT_TRUE(callsignProvider.GetCallsigns().empty());
    }

    TEST_F(LeadWakeCallsignProviderTest, ItSkipsCallsignsIfAirfieldService)
    {
        serviceProviders.SetProvidersForAirfield(
            "EGKK",
            std::vector<std::shared_ptr<ServiceProvision>>{
                std::make_shared<ServiceProvision>(ServiceType::Tower, callsign)});

        plugin.AddAllFlightplansItem({flightplan1, radarTarget1});

        EXPECT_TRUE(callsignProvider.GetCallsigns().empty());
    }

    TEST_F(LeadWakeCallsignProviderTest, ItReturnsNoCallsignsIfNoServicesProvided)
    {
        plugin.AddAllFlightplansItem({flightplan1, radarTarget1});
        EXPECT_TRUE(callsignProvider.GetCallsigns().empty());
    }

    TEST_F(LeadWakeCallsignProviderTest, ItReturnsNoCallsignsIfNoFlightplans)
    {
        serviceProviders.SetProvidersForAirfield(
            "EGLL",
            std::vector<std::shared_ptr<ServiceProvision>>{
                std::make_shared<ServiceProvision>(ServiceType::Tower, callsign)});

        EXPECT_TRUE(callsignProvider.GetCallsigns().empty());
    }

    TEST_F(LeadWakeCallsignProviderTest, SelectingACallsignSetsLeadCallsign)
    {
        callsignProvider.CallsignSelected("BAW123");
        EXPECT_EQ("BAW123", options->LeadAircraft());
    }

    TEST_F(LeadWakeCallsignProviderTest, SelectingACallsignUnsetsFollowingCallsign)
    {
        options->LeadAircraft("BAW456");
        options->FollowingAircraft("BAW999");
        callsignProvider.CallsignSelected("BAW123");
        EXPECT_EQ("BAW123", options->LeadAircraft());
        EXPECT_TRUE(options->FollowingAircraft().empty());
    }
} // namespace UKControllerPluginTest::Wake
