#include "selcal/SelcalParser.h"
#include "selcal/SelcalTagItem.h"
#include "tag/TagData.h"

using testing::NiceMock;
using UKControllerPlugin::Selcal::SelcalParser;
using UKControllerPlugin::Selcal::SelcalTagItem;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

namespace UKControllerPluginTest::Selcal {
    class SelcalTagItemTest : public testing::Test
    {
        public:
        SelcalTagItemTest() : parser(std::make_shared<SelcalParser>()), tagItem(parser)
        {
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
        }

        UKControllerPlugin::Tag::TagData GetTagData(int itemCode)
        {
            return UKControllerPlugin::Tag::TagData(
                this->mockFlightplan,
                this->mockRadarTarget,
                itemCode,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize);
        }

        double fontSize = 24.1;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "";
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        std::shared_ptr<SelcalParser> parser;
        SelcalTagItem tagItem;
    };

    TEST_F(SelcalTagItemTest, ItReturnsBaseTagItemDescription)
    {
        EXPECT_EQ("SELCAL Code", tagItem.GetTagItemDescription(128));
    }

    TEST_F(SelcalTagItemTest, ItReturnsBaseWithSeparatorItemDescription)
    {
        EXPECT_EQ("SELCAL Code With Separator", tagItem.GetTagItemDescription(129));
    }

    TEST_F(SelcalTagItemTest, ItHandlesInvalidCodes)
    {
        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/XXXX"));

        auto tagData = GetTagData(128);
        tagItem.SetTagItemData(tagData);

        EXPECT_EQ("", tagData.GetItemString());
    }

    TEST_F(SelcalTagItemTest, ItSetsTheTagItemFromRemarks)
    {
        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/ABCD"));

        auto tagData = GetTagData(128);
        tagItem.SetTagItemData(tagData);

        EXPECT_EQ("ABCD", tagData.GetItemString());
    }

    TEST_F(SelcalTagItemTest, ItCachesTagItemFromRemarks)
    {
        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/ABCD"));

        auto tagData = GetTagData(128);
        tagItem.SetTagItemData(tagData);

        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/DEFG"));

        EXPECT_EQ("ABCD", tagData.GetItemString());
    }

    TEST_F(SelcalTagItemTest, ItProvidesTheSeparatedItemFromRemarks)
    {
        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/ABCD"));

        auto tagData = GetTagData(129);
        tagItem.SetTagItemData(tagData);

        EXPECT_EQ("AB-CD", tagData.GetItemString());
    }

    TEST_F(SelcalTagItemTest, ItCachesSeparatedItemFromRemarks)
    {
        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/ABCD"));

        auto tagData = GetTagData(129);
        tagItem.SetTagItemData(tagData);

        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/DEFG"));

        EXPECT_EQ("AB-CD", tagData.GetItemString());
    }

    TEST_F(SelcalTagItemTest, FlightplanEventClearsCache)
    {
        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/ABCD"));

        auto tagData = GetTagData(128);
        tagItem.SetTagItemData(tagData);

        tagItem.FlightPlanEvent(this->mockFlightplan, this->mockRadarTarget);

        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/DEFG"));

        tagItem.SetTagItemData(tagData);

        EXPECT_EQ("DEFG", tagData.GetItemString());
    }

    TEST_F(SelcalTagItemTest, FlightplanDisconnectedEventClearsCache)
    {
        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/ABCD"));

        auto tagData = GetTagData(128);
        tagItem.SetTagItemData(tagData);

        tagItem.FlightPlanDisconnectEvent(this->mockFlightplan);

        ON_CALL(mockFlightplan, GetRemarks).WillByDefault(testing::Return("RMK/HI SEL/DEFG"));

        tagItem.SetTagItemData(tagData);

        EXPECT_EQ("DEFG", tagData.GetItemString());
    }

    TEST_F(SelcalTagItemTest, FlightplanDisconnectedHandlesNothingCached)
    {
        EXPECT_NO_THROW(tagItem.FlightPlanDisconnectEvent(this->mockFlightplan));
    }

    TEST_F(SelcalTagItemTest, FlightplanEventHandlesNothingCached)
    {
        EXPECT_NO_THROW(tagItem.FlightPlanEvent(this->mockFlightplan, this->mockRadarTarget));
    }
} // namespace UKControllerPluginTest::Selcal
