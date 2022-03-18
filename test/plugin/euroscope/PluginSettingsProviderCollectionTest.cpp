#include "euroscope/PluginSettingsProviderCollection.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::PluginSettingsProviderCollection;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPluginTest::Euroscope {
    class PluginSettingsProviderCollectionTest : public testing::Test
    {
        public:
        PluginSettingsProviderCollectionTest() : userSettings(settingsProvider), collection(userSettings)
        {
            provider1 = std::make_shared<testing::NiceMock<Euroscope::MockPluginSettingsProvider>>();
            provider2 = std::make_shared<testing::NiceMock<Euroscope::MockPluginSettingsProvider>>();
        }

        testing::NiceMock<Euroscope::MockUserSettingProviderInterface> settingsProvider;
        std::shared_ptr<testing::NiceMock<Euroscope::MockPluginSettingsProvider>> provider1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockPluginSettingsProvider>> provider2;
        UserSetting userSettings;
        PluginSettingsProviderCollection collection;
    };

    TEST_F(PluginSettingsProviderCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(PluginSettingsProviderCollectionTest, ItAddsProviders)
    {
        collection.AddProvider(provider1);
        collection.AddProvider(provider2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(PluginSettingsProviderCollectionTest, ItAssertsAgainstDuplicateProvider)
    {
        collection.AddProvider(provider1);
        collection.AddProvider(provider2);

        EXPECT_DEATH(collection.AddProvider(provider1), "Duplicate PluginSettingsProvider added");
    }

    TEST_F(PluginSettingsProviderCollectionTest, ItLoadsSettingsOnProviders)
    {
        collection.AddProvider(provider1);
        collection.AddProvider(provider2);

        EXPECT_CALL(*provider1, LoadSettings(testing::Ref(userSettings))).Times(1);
        EXPECT_CALL(*provider2, LoadSettings(testing::Ref(userSettings))).Times(1);

        collection.Load();
    }

    TEST_F(PluginSettingsProviderCollectionTest, ItSavesSettingsOnProviders)
    {
        collection.AddProvider(provider1);
        collection.AddProvider(provider2);

        EXPECT_CALL(*provider1, SaveSettings(testing::Ref(userSettings))).Times(1);
        EXPECT_CALL(*provider2, SaveSettings(testing::Ref(userSettings))).Times(1);

        collection.Save();
    }
} // namespace UKControllerPluginTest::Euroscope
