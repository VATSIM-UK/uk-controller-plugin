#include "euroscope/UserSettingAwareCollection.h"

using ::testing::NiceMock;
using ::testing::Ref;
using ::testing::Test;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPluginTest::Euroscope::MockUserSettingAwareInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class UserSettingAwareCollectionTest : public Test
        {
            public:
            UserSettingAwareCollectionTest() : userSetting(mockUserSettingProvider)
            {
                this->awareInterface = std::make_shared<NiceMock<MockUserSettingAwareInterface>>();
            }

            std::shared_ptr<NiceMock<MockUserSettingAwareInterface>> awareInterface;
            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UserSetting userSetting;
            UserSettingAwareCollection collection;
        };

        TEST_F(UserSettingAwareCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->collection.Count());
        }

        TEST_F(UserSettingAwareCollectionTest, ItAddsAHandler)
        {
            this->collection.RegisterHandler(this->awareInterface);
            EXPECT_EQ(1, this->collection.Count());
        }

        TEST_F(UserSettingAwareCollectionTest, ItDoesntAddDuplicates)
        {
            this->collection.RegisterHandler(this->awareInterface);
            this->collection.RegisterHandler(this->awareInterface);
            EXPECT_EQ(1, this->collection.Count());
        }

        TEST_F(UserSettingAwareCollectionTest, SettingUpdateEventCallsTheHandlers)
        {
            EXPECT_CALL(*this->awareInterface, UserSettingsUpdated(Ref(this->userSetting))).Times(1);
            this->collection.RegisterHandler(this->awareInterface);
            this->collection.UserSettingsUpdateEvent(this->userSetting);
        }
    } // namespace Euroscope
} // namespace UKControllerPluginTest
