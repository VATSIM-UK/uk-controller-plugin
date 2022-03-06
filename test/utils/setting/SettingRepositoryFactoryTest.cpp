#include "setting/SettingRepositoryFactory.h"
#include "setting/SettingRepository.h"

using ::testing::Return;
using ::testing::StrictMock;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Setting::SettingRepositoryFactory;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginUtilsTest::Setting {

    TEST(SettingRepositoryFactory, CreateReturnsARepository)
    {
        EXPECT_EQ(0, SettingRepositoryFactory::Create()->CountSettings());
    }
} // namespace UKControllerPluginUtilsTest::Setting
