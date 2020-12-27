#include "pch/pch.h"
#include "wake/WakeCategoryMapper.h"
#include "wake/WakeCategoryEventhandler.h"
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
                        105,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    ),
                    tagData2(
                        flightplan2,
                        radarTarget,
                        105,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    ),
                    tagDataLong(
                        flightplanLongType,
                        radarTarget,
                        105,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    ),
                    tagDataUnknownType(
                        flightplan3,
                        radarTarget,
                        105,
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
                    this->mapper.AddCategoryMapping("B733", "LM");
                    this->mapper.AddCategoryMapping("B744", "H");
                    this->mapper.AddCategoryMapping("123456789012345678", "UM");
                    this->recatMapper.AddCategoryMapping("B733", "D");
                    this->recatMapper.AddCategoryMapping("B744", "B");
                    ON_CALL(this->flightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                    ON_CALL(this->flightplan, GetAircraftType())
                        .WillByDefault(Return("B733"));
                    ON_CALL(this->flightplan2, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                    ON_CALL(this->flightplan2, GetAircraftType())
                        .WillByDefault(Return("B744"));
                    ON_CALL(this->flightplan3, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                    ON_CALL(this->flightplan3, GetAircraftType())
                        .WillByDefault(Return("AN225"));
                    ON_CALL(this->flightplan3, GetIcaoWakeCategory())
                        .WillByDefault(Return("H"));
                    ON_CALL(this->flightplanLongType, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                    ON_CALL(this->flightplanLongType, GetAircraftType())
                        .WillByDefault(Return("123456789012345678"));

                    handler = std::make_shared<WakeCategoryEventHandler>(this->mapper, this->recatMapper);
                }

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan2;
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan3;
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplanLongType;
                NiceMock< MockEuroScopeCRadarTargetInterface> radarTarget;
                TagData tagData1;
                TagData tagData2;
                TagData tagDataLong;
                TagData tagDataUnknownType;
                WakeCategoryMapper mapper;
                WakeCategoryMapper recatMapper;
                std::shared_ptr<WakeCategoryEventHandler> handler;
        };

        TEST_F(WakeCategoryEventHandlerTest, TestItHasATagItemNameForCombined)
        {
            EXPECT_TRUE("Aircraft Type / UK Wake Category" == handler->GetTagItemDescription(105));
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItHasATagItemNameForStandlone)
        {
            EXPECT_TRUE("UK Wake Category" == handler->GetTagItemDescription(112));
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItHasATagItemNameForRecat)
        {
            EXPECT_TRUE("RECAT-EU Category" == handler->GetTagItemDescription(113));
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItHasATagItemNameForUkRecatCombined)
        {
            EXPECT_TRUE("UK / RECAT-EU Combined" == handler->GetTagItemDescription(114));
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItReturnsTheTagItem)
        {
            handler->SetTagItemData(this->tagData1);
            EXPECT_EQ("B733/LM", this->tagData1.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItReturnsTheTagItemWithIcaoTypeIfUnknownAircraftType)
        {
            handler->SetTagItemData(this->tagDataUnknownType);
            EXPECT_EQ("AN225/H", this->tagDataUnknownType.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItTrimsLongAircraftTypes)
        {
            handler->SetTagItemData(this->tagDataLong);
            EXPECT_EQ("123456789012/UM", this->tagDataLong.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItCachesTheResponse)
        {
            handler->SetTagItemData(this->tagData1);
            handler->SetTagItemData(this->tagData2);
            EXPECT_EQ("B733/LM", this->tagData1.GetItemString());
            EXPECT_EQ("B733/LM", this->tagData2.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanEventsClearTheCache)
        {
            handler->SetTagItemData(this->tagData1);
            EXPECT_EQ("B733/LM", this->tagData1.GetItemString());
            handler->FlightPlanEvent(this->flightplan, this->radarTarget);
            handler->SetTagItemData(this->tagData2);
            EXPECT_EQ("B744/H", this->tagData2.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanEventHandlesNonExistantCacheItem)
        {
            handler->FlightPlanEvent(this->flightplan, this->radarTarget);
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanDisconnectsClearTheCache)
        {
            handler->SetTagItemData(this->tagData1);
            EXPECT_EQ("B733/LM", this->tagData1.GetItemString());
            handler->FlightPlanDisconnectEvent(this->flightplan);
            handler->SetTagItemData(this->tagData2);
            EXPECT_EQ("B744/H", this->tagData2.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestFlightplanDisconnectEventHandlesNonExistantCacheItem)
        {
            handler->FlightPlanDisconnectEvent(this->flightplan);
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItReturnsTheStandaloneItem)
        {
            TagData standaloneData = TagData (
                flightplan,
                radarTarget,
                112,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                itemString,
                &euroscopeColourCode,
                &tagColour,
                &fontSize
            );
            handler->SetTagItemData(standaloneData);
            EXPECT_EQ("LM", standaloneData.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItReturnsTheRecatItem)
        {
            TagData recatData = TagData (
                flightplan,
                radarTarget,
                113,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                itemString,
                &euroscopeColourCode,
                &tagColour,
                &fontSize
            );
            handler->SetTagItemData(recatData);
            EXPECT_EQ("D", recatData.GetItemString());
        }

        TEST_F(WakeCategoryEventHandlerTest, TestItReturnsTheUkRecatCombionedItem)
        {
            TagData recatData = TagData (
                flightplan,
                radarTarget,
                114,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                itemString,
                &euroscopeColourCode,
                &tagColour,
                &fontSize
            );
            handler->SetTagItemData(recatData);
            EXPECT_EQ("LM/D", recatData.GetItemString());
        }
    }  // namespace Wake
}  // namespace UKControllerPluginTest
