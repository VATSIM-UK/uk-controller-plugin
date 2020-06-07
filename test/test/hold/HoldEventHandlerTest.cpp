#include "pch/pch.h"
#include "hold/HoldManager.h"
#include "hold/HoldEventHandler.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "plugin/PopupMenuItem.h"
#include "navaids/NavaidCollection.h"
#include "hold/HoldingData.h"
#include "mock/MockApiInterface.h"
#include "mock/MockTaskRunnerInterface.h"

using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldEventHandlerTest : public Test
        {
            public:
                HoldEventHandlerTest(void)
                    : handler(this->manager,this->navaids, this->mockPlugin, 1),
                    manager(mockApi, mockTaskRunner)
                {

                    // Add a FP to the holds initially.
                    NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplanInitial;
                    NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTargetInitial;

                    ON_CALL(mockFlightplanInitial, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    this->manager.AssignAircraftToHold(mockFlightplanInitial, "TIMBA", false);
                }

                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockTaskRunnerInterface> mockTaskRunner;
                NavaidCollection navaids;
                HoldingData holdData = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left", {} };
                std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan;
                std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldManager manager;
                HoldEventHandler handler;
        };

        TEST_F(HoldEventHandlerTest, ItHasATagItemDescription)
        {
            EXPECT_TRUE("Selected Hold" == this->handler.GetTagItemDescription());
        }

        TEST_F(HoldEventHandlerTest, ItReturnsTheSelectedHoldForAnAircraft)
        {
            EXPECT_TRUE("HTIMBA" == this->handler.GetTagItemData(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(HoldEventHandlerTest, ItReturnsNoHoldIfAircraftNotInHold)
        {
            this->manager.UnassignAircraftFromHold("BAW123", false);
            EXPECT_TRUE(
                this->handler.noHold == this->handler.GetTagItemData(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
