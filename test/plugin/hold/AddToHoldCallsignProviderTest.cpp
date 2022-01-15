#include "hold/AddToHoldCallsignProvider.h"
#include "hold/HoldManager.h"
#include "navaids/Navaid.h"

using UKControllerPlugin::Hold::AddToHoldCallsignProvider;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Navaids::Navaid;

namespace UKControllerPluginTest::Hold {
    class AddToHoldCallsignProviderTest : public testing::Test
    {
        public:
        AddToHoldCallsignProviderTest()
            : manager(api, taskRunner), navaid(Navaid{1, "BNN", EuroScopePlugIn::CPosition()}),
              provider(navaid, manager, plugin)
        {
            flightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            ON_CALL(*flightplan1, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*flightplan1, IsTrackedByUser).WillByDefault(testing::Return(true));

            flightplan2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            ON_CALL(*flightplan2, GetCallsign).WillByDefault(testing::Return("EZY456"));
            ON_CALL(*flightplan2, IsTrackedByUser).WillByDefault(testing::Return(true));

            plugin.AddAllFlightplansItem({flightplan1, radarTarget1});
            plugin.AddAllFlightplansItem({flightplan2, radarTarget2});
        }

        testing::NiceMock<TaskManager::MockTaskRunnerInterface> taskRunner;
        testing::NiceMock<Api::MockApiInterface> api;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan2;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget2;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        HoldManager manager;
        Navaid navaid;
        AddToHoldCallsignProvider provider;
    };

    TEST_F(AddToHoldCallsignProviderTest, GetCallsignsReturnsCallsignsNotHolding)
    {
        // EZY is holding elsewhere, so that's ok.
        manager.AssignAircraftToHold("EZY456", "LAM", false);

        EXPECT_EQ(std::set<std::string>({"BAW123", "EZY456"}), provider.GetCallsigns());
    }

    TEST_F(AddToHoldCallsignProviderTest, GetCallsignsDoesntIncludeCallsignsThatAreHolding)
    {
        manager.AssignAircraftToHold("BAW123", "BNN", false);
        manager.AssignAircraftToHold("EZY456", "BNN", false);

        EXPECT_EQ(std::set<std::string>(), provider.GetCallsigns());
    }

    TEST_F(AddToHoldCallsignProviderTest, GetCallsignsDoesntIncludeCallsignsThatAreNotTrackedByUser)
    {
        ON_CALL(*flightplan1, IsTrackedByUser).WillByDefault(testing::Return(false));

        ON_CALL(*flightplan2, IsTrackedByUser).WillByDefault(testing::Return(false));

        EXPECT_EQ(std::set<std::string>(), provider.GetCallsigns());
    }

    TEST_F(AddToHoldCallsignProviderTest, CallsignSelectedAddsToHold)
    {
        EXPECT_CALL(api, AssignAircraftToHold("BAW123", "BNN")).Times(1);

        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan1));

        provider.CallsignSelected("BAW123");
        EXPECT_EQ("BNN", manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
    }

    TEST_F(AddToHoldCallsignProviderTest, CallsignSelectedDoesntAddToHoldIfFlightplanNotTrackedByUser)
    {
        EXPECT_CALL(api, AssignAircraftToHold(testing::_, testing::_)).Times(0);

        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan1));

        ON_CALL(*flightplan1, IsTrackedByUser).WillByDefault(testing::Return(false));

        provider.CallsignSelected("BAW123");
        EXPECT_EQ(nullptr, manager.GetHoldingAircraft("BAW123"));
    }

    TEST_F(AddToHoldCallsignProviderTest, CallsignSelectedDoesntAddToHoldIfFlightplanNotFound)
    {
        EXPECT_CALL(api, AssignAircraftToHold(testing::_, testing::_)).Times(0);

        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));

        provider.CallsignSelected("BAW123");
        EXPECT_EQ(nullptr, manager.GetHoldingAircraft("BAW123"));
    }
} // namespace UKControllerPluginTest::Hold
