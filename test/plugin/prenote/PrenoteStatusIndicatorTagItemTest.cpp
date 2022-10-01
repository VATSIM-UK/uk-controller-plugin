#include "controller/ControllerPosition.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "prenote/PrenoteStatusIndicatorTagItem.h"
#include "time/SystemClock.h"
#include "tag/TagData.h"

using ::testing::NiceMock;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Prenote::PrenoteStatusIndicatorTagItem;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

namespace UKControllerPluginTest::Prenote {
    class PrenoteStatusIndicatorTagItemTest : public ::testing::Test
    {
        public:
        PrenoteStatusIndicatorTagItemTest() : messages(std::make_shared<PrenoteMessageCollection>()), tagItem(messages)
        {
            ON_CALL(this->mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            message1 = std::make_shared<PrenoteMessage>(
                1, "BAW123", "EGGD", "BADIM1X", "EGLL", sendingPosition, receivingPosition, TimeNow());
            message2 = std::make_shared<PrenoteMessage>(
                5, "BAW123", "EGGD", "BADIM1X", "EGLL", sendingPosition, receivingPosition, TimeNow());
        }

        UKControllerPlugin::Tag::TagData GetTagData()
        {
            return UKControllerPlugin::Tag::TagData(
                this->mockFlightplan,
                this->mockRadarTarget,
                1,
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
        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<PrenoteMessageCollection> messages;
        std::shared_ptr<PrenoteMessage> message1;
        std::shared_ptr<PrenoteMessage> message2;
        PrenoteStatusIndicatorTagItem tagItem;
    };

    TEST_F(PrenoteStatusIndicatorTagItemTest, ItHasATagItemDescription)
    {
        EXPECT_EQ("Prenote Messages Status Indicator", tagItem.GetTagItemDescription(128));
    }

    TEST_F(PrenoteStatusIndicatorTagItemTest, ItReturnsBlankIfNoMessages)
    {
        auto tagData = GetTagData();
        tagItem.SetTagItemData(tagData);
        EXPECT_TRUE(tagData.GetItemString().empty());
        EXPECT_EQ(RGB(255, 255, 255), tagData.GetTagColour());
    }

    TEST_F(PrenoteStatusIndicatorTagItemTest, ItDisplaysCounterNoneAcknowledged)
    {
        this->messages->Add(message1);
        this->messages->Add(message2);

        auto tagData = GetTagData();
        tagItem.SetTagItemData(tagData);
        EXPECT_EQ("0/2", tagData.GetItemString());
        EXPECT_EQ(RGB(255, 255, 255), tagData.GetTagColour());
    }

    TEST_F(PrenoteStatusIndicatorTagItemTest, ItDisplaysCounterSomeAcknowledged)
    {
        message1->Acknowledge();
        this->messages->Add(message1);
        this->messages->Add(message2);

        auto tagData = GetTagData();
        tagItem.SetTagItemData(tagData);
        EXPECT_EQ("1/2", tagData.GetItemString());
        EXPECT_EQ(RGB(255, 255, 255), tagData.GetTagColour());
    }

    TEST_F(PrenoteStatusIndicatorTagItemTest, ItDisplaysCounterAllAcknowledged)
    {
        message1->Acknowledge();
        message2->Acknowledge();
        this->messages->Add(message1);
        this->messages->Add(message2);

        auto tagData = GetTagData();
        tagItem.SetTagItemData(tagData);
        EXPECT_EQ("2/2", tagData.GetItemString());
        EXPECT_EQ(RGB(36, 138, 73), tagData.GetTagColour());
    }
} // namespace UKControllerPluginTest::Prenote
