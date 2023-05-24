#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockPluginSettingsProvider.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "releases/DepartureReleaseRequestedEvent.h"
#include "releases/DepartureReleaseRequest.h"
#include "releases/DepartureReleaseRelevanceChecker.h"
#include "releases/SendReleaseRequestedChatAreaMessage.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPluginTest::Releases {

    class MockUserRelevanceChecker : public UKControllerPlugin::Releases::DepartureReleaseRelevanceChecker
    {
        public:
        MOCK_METHOD(
            bool, IsRelevant, (const UKControllerPlugin::Releases::DepartureReleaseRequest&), (const, override));
    };

    class SendReleaseRequestedChatAreaMessageTest : public ::testing::Test
    {
        public:
        SendReleaseRequestedChatAreaMessageTest()
            : releaseRequest(std::make_shared<UKControllerPlugin::Releases::DepartureReleaseRequest>(
                  1, "BAW555", 99, 1, std::chrono::system_clock::now())),
              userSettings(userSettingProvider), sendMessage(relevanceChecker, plugin, userSettings)
        {
        }

        std::shared_ptr<UKControllerPlugin::Releases::DepartureReleaseRequest> releaseRequest;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        testing::NiceMock<Euroscope::MockUserSettingProviderInterface> userSettingProvider;
        std::shared_ptr<testing::NiceMock<MockUserRelevanceChecker>> relevanceChecker =
            std::make_shared<testing::NiceMock<MockUserRelevanceChecker>>();
        UKControllerPlugin::Euroscope::UserSetting userSettings;
        UKControllerPlugin::Releases::SendReleaseRequestedChatAreaMessage sendMessage;
    };

    TEST_F(SendReleaseRequestedChatAreaMessageTest, DoesNotSendMessageIfNotRelevant)
    {
        UKControllerPlugin::Releases::DepartureReleaseRequestedEvent event{releaseRequest};

        ON_CALL(userSettingProvider, KeyExists("sendReleasesToChat")).WillByDefault(testing::Return(true));
        ON_CALL(userSettingProvider, GetKey("sendReleasesToChat")).WillByDefault(testing::Return("1"));
        EXPECT_CALL(*relevanceChecker, IsRelevant(testing::Ref(*releaseRequest))).WillOnce(testing::Return(false));
        EXPECT_CALL(plugin, ChatAreaMessage).Times(0);

        sendMessage.OnEvent(event);
    }

    TEST_F(SendReleaseRequestedChatAreaMessageTest, DoesNotSendMessageIfSettingNotEnabled)
    {
        UKControllerPlugin::Releases::DepartureReleaseRequestedEvent event{releaseRequest};

        ON_CALL(userSettingProvider, KeyExists("sendReleasesToChat")).WillByDefault(testing::Return(false));
        EXPECT_CALL(*relevanceChecker, IsRelevant(testing::Ref(*releaseRequest))).WillOnce(testing::Return(true));
        EXPECT_CALL(plugin, ChatAreaMessage).Times(0);

        sendMessage.OnEvent(event);
    }

    TEST_F(SendReleaseRequestedChatAreaMessageTest, DoesNotSendMessageIfSettingDisabled)
    {
        UKControllerPlugin::Releases::DepartureReleaseRequestedEvent event{releaseRequest};

        ON_CALL(userSettingProvider, KeyExists("sendReleasesToChat")).WillByDefault(testing::Return(true));
        ON_CALL(userSettingProvider, GetKey("sendReleasesToChat")).WillByDefault(testing::Return("0"));
        EXPECT_CALL(*relevanceChecker, IsRelevant(testing::Ref(*releaseRequest))).WillOnce(testing::Return(true));
        EXPECT_CALL(plugin, ChatAreaMessage).Times(0);

        sendMessage.OnEvent(event);
    }

    TEST_F(SendReleaseRequestedChatAreaMessageTest, SendsMessageIfRelevantAndSettingEnabled)
    {
        UKControllerPlugin::Releases::DepartureReleaseRequestedEvent event{releaseRequest};

        ON_CALL(userSettingProvider, KeyExists("sendReleasesToChat")).WillByDefault(testing::Return(true));
        ON_CALL(userSettingProvider, GetKey("sendReleasesToChat")).WillByDefault(testing::Return("1"));
        EXPECT_CALL(*relevanceChecker, IsRelevant(testing::Ref(*releaseRequest))).WillOnce(testing::Return(true));
        EXPECT_CALL(
            plugin,
            ChatAreaMessage(
                "UKCP_COORDINATION", "UKCP", "Departure release requested for BAW555.", true, true, true, true, true))
            .Times(1);

        sendMessage.OnEvent(event);
    }
} // namespace UKControllerPluginTest::Releases
