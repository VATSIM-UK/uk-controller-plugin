#include "controller/ControllerPosition.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/SendPrenoteCancelledChatAreaMessage.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::SendPrenoteCancelledChatAreaMessage;

namespace UKControllerPluginTest::Prenote {
    class SendPrenoteCancelledChatAreaMessageTest : public testing::Test
    {
        public:
        SendPrenoteCancelledChatAreaMessageTest()
            : mockPrenoteRelevance(std::make_shared<testing::NiceMock<MockPrenoteUserRelevanceChecker>>()),
              sendMessage(mockPrenoteRelevance, plugin)
        {
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
        }

        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<testing::NiceMock<MockPrenoteUserRelevanceChecker>> mockPrenoteRelevance;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        SendPrenoteCancelledChatAreaMessage sendMessage;
    };

    TEST_F(SendPrenoteCancelledChatAreaMessageTest, ItSendsChatAreaMessageOnPrenoteMessageCancelled)
    {
        const PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            receivingPosition,
            std::chrono::system_clock::now());
        EXPECT_CALL(
            plugin,
            ChatAreaMessage(
                "UKCP_COORDINATION",
                "UKCP",
                "Prenote message for BAW123 from EGKK_TWR has been cancelled.",
                true,
                true,
                true,
                true,
                true))
            .Times(1);
        EXPECT_CALL(*mockPrenoteRelevance, IsRelevant(testing::Ref(message))).Times(1).WillOnce(testing::Return(true));
        sendMessage.MessageCancelled(message);
    }

    TEST_F(SendPrenoteCancelledChatAreaMessageTest, ItDoesntSendMessageIfNotRelevantToController)
    {
        const PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            receivingPosition,
            std::chrono::system_clock::now());
        EXPECT_CALL(plugin, ChatAreaMessage).Times(0);
        EXPECT_CALL(*mockPrenoteRelevance, IsRelevant(testing::Ref(message))).Times(1).WillOnce(testing::Return(false));
        sendMessage.MessageCancelled(message);
    }
} // namespace UKControllerPluginTest::Prenote
