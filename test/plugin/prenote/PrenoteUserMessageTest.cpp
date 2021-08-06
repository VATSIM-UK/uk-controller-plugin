#include "pch/pch.h"
#include "prenote/PrenoteUserMessage.h"
#include "controller/ActiveCallsign.h"
#include "prenote/DeparturePrenote.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Prenote::PrenoteUserMessage;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Prenote::DeparturePrenote;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPlugin::Controller::ControllerPosition;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Prenote {

        class PrenoteMessageTest : public Test
        {
            public:

                void SetUp(void)
                {
                    ON_CALL(mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    callsign.reset(
                        new ActiveCallsign(
                            "LTC_SE_CTR",
                            "Testy McTestface",
                            ControllerPosition(1, "LTC_SE_CTR", 121.225, {}, true, false)
                        )
                    );
                    departurePrenote.reset(new DeparturePrenote(NULL, "EGKK", "BIG2X"));
                    prenoteMessage.reset(
                        new PrenoteUserMessage(*this->departurePrenote, *this->callsign, this->mockFlightplan)
                    );
                };

                std::unique_ptr<PrenoteUserMessage> prenoteMessage;
                std::unique_ptr<ActiveCallsign> callsign;
                std::unique_ptr<DeparturePrenote> departurePrenote;
                NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        };

        TEST_F(PrenoteMessageTest, ItWritesToThePrenoteHandler)
        {
            ASSERT_TRUE(this->prenoteMessage->MessageHandler() == this->prenoteMessage->handler);
        }

        TEST_F(PrenoteMessageTest, ItSendsFromThePlugin)
        {
            ASSERT_TRUE(this->prenoteMessage->MessageSender() == this->prenoteMessage->sender);
        }

        TEST_F(PrenoteMessageTest, ItCompilesAMessageToSend)
        {
            ASSERT_TRUE(
                this->prenoteMessage->MessageString() == "Prenote to LTC_SE_CTR required for BAW123 (EGKK, BIG2X)"
            );
        }

        TEST_F(PrenoteMessageTest, ItShowsTheMessageHandler)
        {
            ASSERT_TRUE(this->prenoteMessage->MessageShowHandler());
        }

        TEST_F(PrenoteMessageTest, ItMarksTheHandlerAsUnread)
        {
            ASSERT_TRUE(this->prenoteMessage->MessageMarkUnread());
        }

        TEST_F(PrenoteMessageTest, ItOverridesBusyStatus)
        {
            ASSERT_TRUE(this->prenoteMessage->MessageOverrideBusy());
        }

        TEST_F(PrenoteMessageTest, ItFlashesTheHandler)
        {
            ASSERT_TRUE(this->prenoteMessage->MessageFlashHandler());
        }

        TEST_F(PrenoteMessageTest, ItRequiresAConfirmation)
        {
            ASSERT_TRUE(this->prenoteMessage->MessageRequiresConfirm());
        }
    }  // namespace Prenote
}  // namespace UKControllerPluginTest
