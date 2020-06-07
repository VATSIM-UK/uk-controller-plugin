#include "pch/pch.h"
#include "hold/HoldManager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "hold/HoldingAircraft.h"
#include "mock/MockApiInterface.h"
#include "mock/MockTaskRunnerInterface.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Hold::HoldingAircraft;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldManagerTest : public Test
        {
            public:
                HoldManagerTest(void)
                    : manager(mockApi, mockTaskRunner)
                {

                    ON_CALL(mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    ON_CALL(mockFlightplan, GetClearedAltitude())
                        .WillByDefault(Return(8000));

                    ON_CALL(mockRadarTarget, GetFlightLevel())
                        .WillByDefault(Return(9000));

                    ON_CALL(mockRadarTarget, GetVerticalSpeed())
                        .WillByDefault(Return(300));
                }

                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockTaskRunnerInterface> mockTaskRunner;
                NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
                NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldManager manager;
        };
    }  // namespace Hold
}  // namespace UKControllerPluginTest
