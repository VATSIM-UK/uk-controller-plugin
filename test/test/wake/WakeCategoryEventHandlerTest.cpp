#include "pch/pch.h"
#include "wake/WakeCategoryMapper.h"
#include "wake/WakeCategoryEventHandler.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"

using UKControllerPlugin::Wake::WakeCategoryMapper;
using UKControllerPlugin::Wake::WakeCategoryEventHandler;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Wake {
        class WakeCategoryEventHandlerTest : public Test
        {
            public:
                void SetUp()
                {
                    this->mapper.AddTypeMapping("B733", "LM");
                    this->mapper.AddTypeMapping("B744", "H");
                    ON_CALL(this->flightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                    ON_CALL(this->flightplan, GetAircraftType())
                        .WillByDefault(Return("B733"));
                    ON_CALL(this->flightplan2, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                    ON_CALL(this->flightplan2, GetAircraftType())
                        .WillByDefault(Return("B744"));
                }

                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan2;
                NiceMock< MockEuroScopeCRadarTargetInterface> radarTarget;
                WakeCategoryMapper mapper;
        };

        TEST_F(WakeCategoryEventHandlerTest, TestItHasATagItemName)
        {
            WakeCategoryEventHandler handler(this->mapper);
            EXPECT_TRUE("Aircraft Type / UK Wake Category" == handler.GetTagItemDescription());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItReturnsTheTagItem)
        {
            WakeCategoryEventHandler handler(this->mapper);
            EXPECT_TRUE("B733/LM" == handler.GetTagItemData(this->flightplan, this->radarTarget));
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItCachesTheResponse)
        {
            WakeCategoryEventHandler handler(this->mapper);
            EXPECT_TRUE("B733/LM" == handler.GetTagItemData(this->flightplan, this->radarTarget));
            EXPECT_TRUE("B733/LM" == handler.GetTagItemData(this->flightplan2, this->radarTarget));
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanEventsClearTheCache)
        {
            WakeCategoryEventHandler handler(this->mapper);
            EXPECT_TRUE("B733/LM" == handler.GetTagItemData(this->flightplan, this->radarTarget));
            handler.FlightPlanEvent(this->flightplan, this->radarTarget);
            EXPECT_TRUE("B744/H" == handler.GetTagItemData(this->flightplan2, this->radarTarget));
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanDisconnectsClearTheCache)
        {
            WakeCategoryEventHandler handler(this->mapper);
            EXPECT_TRUE("B733/LM" == handler.GetTagItemData(this->flightplan, this->radarTarget));
            handler.FlightPlanDisconnectEvent(this->flightplan);
            EXPECT_TRUE("B744/H" == handler.GetTagItemData(this->flightplan2, this->radarTarget));
        }

    }  // namespace Wake
}  // namespace UKControllerPluginTest
