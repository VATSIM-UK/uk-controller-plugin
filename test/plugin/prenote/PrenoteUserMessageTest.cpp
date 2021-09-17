#include "prenote/PrenoteUserMessage.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "prenote/DeparturePrenote.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::DeparturePrenote;
using UKControllerPlugin::Prenote::PrenoteUserMessage;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;

namespace UKControllerPluginTest::Prenote {

    class PrenoteUseMessageTest : public Test
    {
        public:
        void SetUp() override
        {
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(Return("BAW123"));

            this->callsign = std::make_unique<ActiveCallsign>(
                "LTC_SE_CTR", "Testy McTestface", ControllerPosition(1, "LTC_SE_CTR", 121.225, {}, true, false));
            this->departurePrenote = std::make_unique<DeparturePrenote>(nullptr, "EGKK", "BIG2X");
            this->prenoteMessage =
                std::make_unique<PrenoteUserMessage>(*this->departurePrenote, *this->callsign, this->mockFlightplan);
        };
#
        std::unique_ptr<PrenoteUserMessage> prenoteMessage;
        std::unique_ptr<ActiveCallsign> callsign;
        std::unique_ptr<DeparturePrenote> departurePrenote;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
    };

    TEST_F(PrenoteUseMessageTest, ItWritesToThePrenoteHandler)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageHandler() == "Prenote");
    }

    TEST_F(PrenoteUseMessageTest, ItSendsFromThePlugin)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageSender() == "UKCP");
    }

    TEST_F(PrenoteUseMessageTest, ItCompilesAMessageToSend)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageString() == "Prenote to LTC_SE_CTR required for BAW123 (EGKK, BIG2X)");
    }

    TEST_F(PrenoteUseMessageTest, ItShowsTheMessageHandler)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageShowHandler());
    }

    TEST_F(PrenoteUseMessageTest, ItMarksTheHandlerAsUnread)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageMarkUnread());
    }

    TEST_F(PrenoteUseMessageTest, ItOverridesBusyStatus)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageOverrideBusy());
    }

    TEST_F(PrenoteUseMessageTest, ItFlashesTheHandler)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageFlashHandler());
    }

    TEST_F(PrenoteUseMessageTest, ItRequiresAConfirmation)
    {
        ASSERT_TRUE(this->prenoteMessage->MessageRequiresConfirm());
    }
} // namespace UKControllerPluginTest::Prenote
