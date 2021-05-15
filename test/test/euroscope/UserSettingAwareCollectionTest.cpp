#include "pch/pch.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "mock/MockUserSettingAwareInterface.h"

using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingAwareInterface;
using ::testing::NiceMock;
using ::testing::Ref;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class UserSettingAwareCollectionTest : public Test
        {
            public:
                UserSettingAwareCollectionTest()
                    : userSetting(mockUserSettingProvider)
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
            EXPECT_CALL(*this->awareInterface, UserSettingsUpdated(Ref(this->userSetting)))
                .Times(1);
            this->collection.RegisterHandler(this->awareInterface);
            this->collection.UserSettingsUpdateEvent(this->userSetting);
        }
    }  // namespace Euroscope
} // namespace UKControllerPluginTest
