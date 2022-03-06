#include "wake/FollowingWakeCallsignProvider.h"
#include "wake/WakeCalculatorOptions.h"

using UKControllerPlugin::Wake::FollowingWakeCallsignProvider;
using UKControllerPlugin::Wake::WakeCalculatorOptions;

namespace UKControllerPluginTest::Wake {
    class FollowingWakeCallsignProviderTest : public testing::Test
    {
        public:
        FollowingWakeCallsignProviderTest()
            : options(std::make_shared<WakeCalculatorOptions>()), callsignProvider(plugin, options)
        {
            flightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            flightplan2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            radarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
            radarTarget2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

            ON_CALL(*flightplan1, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*flightplan1, GetOrigin).WillByDefault(testing::Return("EGLL"));
            ON_CALL(*flightplan1, GetDestination).WillByDefault(testing::Return("EGLL"));

            ON_CALL(*flightplan2, GetCallsign).WillByDefault(testing::Return("BAW456"));

            options->LeadAircraft("BAW123");
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan2;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget2;
        std::shared_ptr<WakeCalculatorOptions> options;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        FollowingWakeCallsignProvider callsignProvider;
    };

    TEST_F(FollowingWakeCallsignProviderTest, ItReturnsRelevantCallsigns)
    {
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan1));

        ON_CALL(*flightplan2, GetOrigin).WillByDefault(testing::Return("EGLL"));
        plugin.AddAllFlightplansItem({flightplan1, radarTarget1});
        plugin.AddAllFlightplansItem({flightplan2, radarTarget2});

        EXPECT_EQ(std::set<std::string>{"BAW456"}, callsignProvider.GetCallsigns());
    }

    TEST_F(FollowingWakeCallsignProviderTest, ItReturnsRelevantArrivalCallsigns)
    {
        options->ToggleArrivals();
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan1));

        ON_CALL(*flightplan2, GetDestination).WillByDefault(testing::Return("EGLL"));
        plugin.AddAllFlightplansItem({flightplan1, radarTarget1});
        plugin.AddAllFlightplansItem({flightplan2, radarTarget2});

        EXPECT_EQ(std::set<std::string>{"BAW456"}, callsignProvider.GetCallsigns());
    }

    TEST_F(FollowingWakeCallsignProviderTest, ItDoesntReturnWrongArrivalCallsigns)
    {
        options->ToggleArrivals();
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan1));

        ON_CALL(*flightplan2, GetDestination).WillByDefault(testing::Return("EGKK"));
        plugin.AddAllFlightplansItem({flightplan1, radarTarget1});
        plugin.AddAllFlightplansItem({flightplan2, radarTarget2});

        EXPECT_EQ(std::set<std::string>{}, callsignProvider.GetCallsigns());
    }

    TEST_F(FollowingWakeCallsignProviderTest, ItIgnoresCallsignIfOriginDoesntMatchLead)
    {
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan1));

        ON_CALL(*flightplan2, GetOrigin).WillByDefault(testing::Return("EGKK"));
        plugin.AddAllFlightplansItem({flightplan1, radarTarget1});
        plugin.AddAllFlightplansItem({flightplan2, radarTarget2});

        EXPECT_TRUE(callsignProvider.GetCallsigns().empty());
    }

    TEST_F(FollowingWakeCallsignProviderTest, ItReturnsEmptyIfNoFlightplanForLead)
    {
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));
        plugin.AddAllFlightplansItem({flightplan1, radarTarget1});
        plugin.AddAllFlightplansItem({flightplan2, radarTarget2});

        EXPECT_TRUE(callsignProvider.GetCallsigns().empty());
    }

    TEST_F(FollowingWakeCallsignProviderTest, SelectingACallsignSetsFollowingCallsign)
    {
        options->LeadAircraft("BAW456");
        options->FollowingAircraft("BAW999");
        callsignProvider.CallsignSelected("BAW123");
        EXPECT_EQ("BAW123", options->FollowingAircraft());
        EXPECT_EQ("BAW456", options->LeadAircraft());
    }
} // namespace UKControllerPluginTest::Wake
