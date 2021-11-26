#include "prenote/PrenoteUserMessage.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteUserMessage;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;

namespace UKControllerPluginTest::Prenote {

    class PrenoteUserMessageTest : public Test
    {
        public:
        PrenoteUserMessageTest()
            : callsign(
                  "LTC_SE_CTR",
                  "Testy McTestface",
                  ControllerPosition(1, "LTC_SE_CTR", 121.225, {}, true, false),
                  false)
        {
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(Return("BAW123"));
            ON_CALL(mockFlightplan, GetOrigin).WillByDefault(Return("EGGD"));
            ON_CALL(mockFlightplan, GetSidName).WillByDefault(Return("BADIM1X"));

            prenoteMessage = std::make_unique<PrenoteUserMessage>(callsign, mockFlightplan);
        }

        ActiveCallsign callsign;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        std::unique_ptr<PrenoteUserMessage> prenoteMessage;
    };

    TEST_F(PrenoteUserMessageTest, ItWritesToThePrenoteHandler)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageHandler() == "PRENOTE");
    }

    TEST_F(PrenoteUserMessageTest, ItSendsFromThePlugin)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageSender() == "UKCP");
    }

    TEST_F(PrenoteUserMessageTest, ItCompilesAMessageToSend)
    {
        ASSERT_EQ(this->prenoteMessage->MessageString(), "Prenote to LTC_SE_CTR required for BAW123 (EGGD/BADIM1X)");
    }

    TEST_F(PrenoteUserMessageTest, ItCompilesAMessageToSendWithoutSid)
    {
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(Return(""));
        PrenoteUserMessage message2(callsign, mockFlightplan);
        ASSERT_EQ(message2.MessageString(), "Prenote to LTC_SE_CTR required for BAW123 (EGGD)");
    }

    TEST_F(PrenoteUserMessageTest, ItShowsTheMessageHandler)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageShowHandler());
    }

    TEST_F(PrenoteUserMessageTest, ItMarksTheHandlerAsUnread)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageMarkUnread());
    }

    TEST_F(PrenoteUserMessageTest, ItOverridesBusyStatus)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageOverrideBusy());
    }

    TEST_F(PrenoteUserMessageTest, ItFlashesTheHandler)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageFlashHandler());
    }

    TEST_F(PrenoteUserMessageTest, ItRequiresAConfirmation)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageRequiresConfirm());
    }
} // namespace UKControllerPluginTest::Prenote
