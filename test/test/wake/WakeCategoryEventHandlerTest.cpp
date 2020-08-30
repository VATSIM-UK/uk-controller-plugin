#include "pch/pch.h"
#include "wake/WakeCategoryMapper.h"
#include "wake/WakeCategoryEventHandler.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "tag/TagData.h"

using UKControllerPlugin::Wake::WakeCategoryMapper;
using UKControllerPlugin::Wake::WakeCategoryEventHandler;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Tag::TagData;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Wake {
        class WakeCategoryEventHandlerTest : public Test
        {
            public:

                WakeCategoryEventHandlerTest()
                    : tagData1(
                        flightplan,
                        radarTarget,
                        1,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    ),
                    tagData2(
                        flightplan2,
                        radarTarget,
                        1,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    ),
                    tagDataLong(
                        flightplanLongType,
                        radarTarget,
                        1,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    )
                {

                }

                void SetUp()
                {
                    this->mapper.AddTypeMapping("B733", "LM");
                    this->mapper.AddTypeMapping("B744", "H");
                    this->mapper.AddTypeMapping("123456789012345678", "UM");
                    ON_CALL(this->flightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                    ON_CALL(this->flightplan, GetAircraftType())
                        .WillByDefault(Return("B733"));
                    ON_CALL(this->flightplan2, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                    ON_CALL(this->flightplan2, GetAircraftType())
                        .WillByDefault(Return("B744"));
                    ON_CALL(this->flightplanLongType, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                    ON_CALL(this->flightplanLongType, GetAircraftType())
                        .WillByDefault(Return("123456789012345678"));
                }

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan2;
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplanLongType;
                NiceMock< MockEuroScopeCRadarTargetInterface> radarTarget;
                TagData tagData1;
                TagData tagData2;
                TagData tagDataLong;
                WakeCategoryMapper mapper;
        };

        TEST_F(WakeCategoryEventHandlerTest, TestItHasATagItemName)
        {
            WakeCategoryEventHandler handler(this->mapper);
            EXPECT_TRUE("Aircraft Type / UK Wake Category" == handler.GetTagItemDescription(0));
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItReturnsTheTagItem)
        {
            WakeCategoryEventHandler handler(this->mapper);
            handler.SetTagItemData(this->tagData1);
            EXPECT_EQ("B733/LM", this->tagData1.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItTrimsLongAircraftTypes)
        {
            WakeCategoryEventHandler handler(this->mapper);
            handler.SetTagItemData(this->tagDataLong);
            EXPECT_EQ("123456789012/UM", this->tagDataLong.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItCachesTheResponse)
        {
            WakeCategoryEventHandler handler(this->mapper);
            handler.SetTagItemData(this->tagData1);
            handler.SetTagItemData(this->tagData2);
            EXPECT_EQ("B733/LM", this->tagData1.GetItemString());
            EXPECT_EQ("B733/LM", this->tagData2.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanEventsClearTheCache)
        {
            WakeCategoryEventHandler handler(this->mapper);
            handler.SetTagItemData(this->tagData1);
            EXPECT_EQ("B733/LM", this->tagData1.GetItemString());
            handler.FlightPlanEvent(this->flightplan, this->radarTarget);
            handler.SetTagItemData(this->tagData2);
            EXPECT_EQ("B744/H", this->tagData2.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanEventHandlesNonExistantCacheItem)
        {
            WakeCategoryEventHandler handler(this->mapper);
            handler.FlightPlanEvent(this->flightplan, this->radarTarget);
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanDisconnectsClearTheCache)
        {
            WakeCategoryEventHandler handler(this->mapper);
            handler.SetTagItemData(this->tagData1);
            EXPECT_EQ("B733/LM", this->tagData1.GetItemString());
            handler.FlightPlanDisconnectEvent(this->flightplan);
            handler.SetTagItemData(this->tagData2);
            EXPECT_EQ("B744/H", this->tagData2.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanDisconnectEventHandlesNonExistantCacheItem)
        {
            WakeCategoryEventHandler handler(this->mapper);
            handler.FlightPlanDisconnectEvent(this->flightplan);
        }

    }  // namespace Wake
}  // namespace UKControllerPluginTest
