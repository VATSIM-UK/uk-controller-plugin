#include "setting/SettingRepository.h"

using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPluginTest::Setting::MockSettingProvider;

namespace UKControllerPluginUtilsTest::Setting {

    class SettingRepositoryTest : public testing::Test
    {
        public:
        SettingRepositoryTest()
            : provider1Settings({"setting1", "setting2"}), provider2Settings({"setting3"}),
              mockProvider1(std::make_shared<testing::NiceMock<MockSettingProvider>>()),
              mockProvider2(std::make_shared<testing::NiceMock<MockSettingProvider>>())
        {
            ON_CALL(*mockProvider1, Provides).WillByDefault(testing::ReturnRef(provider1Settings));

            ON_CALL(*mockProvider2, Provides).WillByDefault(testing::ReturnRef(provider2Settings));
        }

        std::set<std::string> provider1Settings;
        std::set<std::string> provider2Settings;
        std::shared_ptr<testing::NiceMock<MockSettingProvider>> mockProvider1;
        std::shared_ptr<testing::NiceMock<MockSettingProvider>> mockProvider2;
        SettingRepository repository;
    };

    TEST_F(SettingRepositoryTest, ItStartsWithNoSettings)
    {
        EXPECT_EQ(0, repository.CountSettings());
    }

    TEST_F(SettingRepositoryTest, ItAddsSettingProviders)
    {
        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);
        EXPECT_EQ(3, repository.CountSettings());
        EXPECT_TRUE(repository.HasSetting("setting1"));
        EXPECT_TRUE(repository.HasSetting("setting2"));
        EXPECT_TRUE(repository.HasSetting("setting3"));
    }

    TEST_F(SettingRepositoryTest, ItDoesntAddDuplicateProviders)
    {
        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);
        repository.AddProvider(mockProvider2);
        repository.AddProvider(mockProvider2);
        EXPECT_EQ(3, repository.CountSettings());
        EXPECT_TRUE(repository.HasSetting("setting1"));
        EXPECT_TRUE(repository.HasSetting("setting2"));
        EXPECT_TRUE(repository.HasSetting("setting3"));
    }

    // TODO: Add remaining tests
} // namespace UKControllerPluginUtilsTest::Setting
