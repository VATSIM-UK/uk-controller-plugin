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

    TEST_F(SettingRepositoryTest, ItReturnsDefaultValueIfNoProviderForSetting)
    {
        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);

        EXPECT_EQ("abc", repository.GetSetting("test1", "abc"));
    }

    TEST_F(SettingRepositoryTest, ItReturnsEmptyValueIfNoProviderForSettingAndNoDefaultProvided)
    {
        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);

        EXPECT_EQ("", repository.GetSetting("test1"));
    }

    TEST_F(SettingRepositoryTest, ItReturnsDefaultValueIfProviderReturnsEmpty)
    {
        EXPECT_CALL(*mockProvider1, Get("setting1")).Times(1).WillOnce(testing::Return(""));

        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);

        EXPECT_EQ("abc", repository.GetSetting("setting1", "abc"));
    }

    TEST_F(SettingRepositoryTest, ItReturnsValueFromProvider)
    {
        EXPECT_CALL(*mockProvider1, Get("setting1")).Times(1).WillOnce(testing::Return("123"));

        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);

        EXPECT_EQ("123", repository.GetSetting("setting1"));
    }

    TEST_F(SettingRepositoryTest, ItDoesntUpdateSettingIfNoProvider)
    {
        EXPECT_CALL(*mockProvider1, Save(testing::_, testing::_)).Times(0);

        EXPECT_CALL(*mockProvider2, Save(testing::_, testing::_)).Times(0);

        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);

        repository.UpdateSetting("setting4", "abc");
    }

    TEST_F(SettingRepositoryTest, ItUpdatesSetting)
    {
        EXPECT_CALL(*mockProvider1, Save(testing::_, testing::_)).Times(0);

        EXPECT_CALL(*mockProvider2, Save("setting3", "abc")).Times(1);

        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);

        repository.UpdateSetting("setting3", "abc");
    }

    TEST_F(SettingRepositoryTest, ItDoesntReloadSettingIfNoProvider)
    {
        EXPECT_CALL(*mockProvider1, Reload()).Times(0);

        EXPECT_CALL(*mockProvider2, Reload()).Times(0);

        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);

        repository.ReloadSetting("setting4");
    }

    TEST_F(SettingRepositoryTest, ItReloadsSetting)
    {
        EXPECT_CALL(*mockProvider1, Reload()).Times(0);

        EXPECT_CALL(*mockProvider2, Reload()).Times(1);

        repository.AddProvider(mockProvider1);
        repository.AddProvider(mockProvider2);

        repository.ReloadSetting("setting3");
    }
} // namespace UKControllerPluginUtilsTest::Setting
