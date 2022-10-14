#include "controller/ControllerPosition.h"
#include "euroscope/UserSetting.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/SendPrenoteTimeoutChatAreaMessage.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::SendPrenoteTimeoutChatAreaMessage;

namespace UKControllerPluginTest::Prenote {
    class SendPrenoteTimeoutChatAreaMessageTest : public testing::Test
    {
        public:
        SendPrenoteTimeoutChatAreaMessageTest()
            : userSettings(userSettingProvider),
              mockPrenoteRelevance(std::make_shared<testing::NiceMock<MockPrenoteUserRelevanceChecker>>()),
              sendMessage(mockPrenoteRelevance, plugin, userSettings)
        {
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
        }

        testing::NiceMock<Euroscope::MockUserSettingProviderInterface> userSettingProvider;
        UserSetting userSettings;
        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<testing::NiceMock<MockPrenoteUserRelevanceChecker>> mockPrenoteRelevance;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        SendPrenoteTimeoutChatAreaMessage sendMessage;
    };

    TEST_F(SendPrenoteTimeoutChatAreaMessageTest, ItSendsChatAreaMessageOnPrenoteMessageTimedOut)
    {
        ON_CALL(userSettingProvider, KeyExists("sendPrenotesToChat")).WillByDefault(testing::Return(true));
        ON_CALL(userSettingProvider, GetKey("sendPrenotesToChat")).WillByDefault(testing::Return("1"));
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
                "Prenote message to EGKK_F_APP for BAW123 has timed out without acknowledgement.",
                true,
                true,
                true,
                true,
                true))
            .Times(1);
        EXPECT_CALL(*mockPrenoteRelevance, IsRelevant(testing::Ref(message))).Times(1).WillOnce(testing::Return(true));
        sendMessage.MessageTimeout(message);
    }

    TEST_F(SendPrenoteTimeoutChatAreaMessageTest, ItDoesntSendMessageIfPrenoteHasBeenAcknowledged)
    {
        ON_CALL(userSettingProvider, KeyExists("sendPrenotesToChat")).WillByDefault(testing::Return(true));
        ON_CALL(userSettingProvider, GetKey("sendPrenotesToChat")).WillByDefault(testing::Return("1"));

        PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            receivingPosition,
            std::chrono::system_clock::now());
        message.Acknowledge();
        EXPECT_CALL(plugin, ChatAreaMessage).Times(0);
        EXPECT_CALL(*mockPrenoteRelevance, IsRelevant(testing::Ref(message))).Times(0);
        sendMessage.MessageTimeout(message);
    }

    TEST_F(SendPrenoteTimeoutChatAreaMessageTest, ItDoesntSendMessageIfNotUserDoesntWantMessages)
    {
        ON_CALL(userSettingProvider, KeyExists("sendPrenotesToChat")).WillByDefault(testing::Return(true));
        ON_CALL(userSettingProvider, GetKey("sendPrenotesToChat")).WillByDefault(testing::Return("0"));

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
        EXPECT_CALL(*mockPrenoteRelevance, IsRelevant(testing::Ref(message))).Times(0);
        sendMessage.MessageTimeout(message);
    }

    TEST_F(SendPrenoteTimeoutChatAreaMessageTest, ItDoesntSendMessageIfNotRelevantToController)
    {
        ON_CALL(userSettingProvider, KeyExists("sendPrenotesToChat")).WillByDefault(testing::Return(true));
        ON_CALL(userSettingProvider, GetKey("sendPrenotesToChat")).WillByDefault(testing::Return("1"));
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
        sendMessage.MessageTimeout(message);
    }
} // namespace UKControllerPluginTest::Prenote
