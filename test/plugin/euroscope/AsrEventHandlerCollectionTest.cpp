#include "euroscope/AsrEventHandlerCollection.h"

using ::testing::Ref;
using ::testing::StrictMock;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::RadarScreen::MockAsrEventHandlerInterface;

namespace UKControllerPluginTest {
    namespace Euroscope {

        TEST(AsrEventHandlerCollection, AsrLoadedEventCallsCorrectHandlerMethod)
        {
            AsrEventHandlerCollection collection;
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UserSetting userSetting(mockUserSettingProvider);
            std::shared_ptr<StrictMock<MockAsrEventHandlerInterface>> mockHandler(
                new StrictMock<MockAsrEventHandlerInterface>);

            EXPECT_CALL(*mockHandler, AsrLoadedEvent(Ref(userSetting))).Times(1);

            collection.RegisterHandler(mockHandler);
            collection.AsrLoadedEvent(userSetting);
        }

        TEST(AsrEventHandlerCollection, AsrClosingEventEventCallsCorrectHandlerMethod)
        {
            AsrEventHandlerCollection collection;
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UserSetting userSetting(mockUserSettingProvider);
            std::shared_ptr<StrictMock<MockAsrEventHandlerInterface>> mockHandler(
                new StrictMock<MockAsrEventHandlerInterface>);

            EXPECT_CALL(*mockHandler, AsrClosingEvent(Ref(userSetting))).Times(1);

            collection.RegisterHandler(mockHandler);
            collection.AsrClosingEvent(userSetting);
        }

        TEST(AsrEventHandlerCollection, CountHandlersReturnsNumberOfRegisteredHandlers)
        {
            AsrEventHandlerCollection collection;
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UserSetting userSetting(mockUserSettingProvider);
            std::shared_ptr<StrictMock<MockAsrEventHandlerInterface>> mockHandler1(
                new StrictMock<MockAsrEventHandlerInterface>);
            std::shared_ptr<StrictMock<MockAsrEventHandlerInterface>> mockHandler2(
                new StrictMock<MockAsrEventHandlerInterface>);
            std::shared_ptr<StrictMock<MockAsrEventHandlerInterface>> mockHandler3(
                new StrictMock<MockAsrEventHandlerInterface>);

            collection.RegisterHandler(mockHandler1);
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterHandler(mockHandler2);
            EXPECT_EQ(2, collection.CountHandlers());
            collection.RegisterHandler(mockHandler3);
            EXPECT_EQ(3, collection.CountHandlers());
        }

        TEST(AsrEventHandlerCollection, RegisterHandlerDoesNotAddDuplicates)
        {
            AsrEventHandlerCollection collection;
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UserSetting userSetting(mockUserSettingProvider);
            std::shared_ptr<StrictMock<MockAsrEventHandlerInterface>> mockHandler(
                new StrictMock<MockAsrEventHandlerInterface>);

            collection.RegisterHandler(mockHandler);
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterHandler(mockHandler);
            EXPECT_EQ(1, collection.CountHandlers());
        }
    } // namespace Euroscope
} // namespace UKControllerPluginTest
