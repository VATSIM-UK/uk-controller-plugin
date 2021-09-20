#include "euroscope/LoadDefaultUserSettings.h"
#include "euroscope/UserSetting.h"
#include "euroscope/GeneralSettingsEntries.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::LoadDefaultUserSettings;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class LoadDefaultUserSettingsTest : public Test
        {
            public:
            LoadDefaultUserSettingsTest() : userSetting(mockUserSettingProvider)
            {
            }

            void SetExpectation(std::string key, std::string description, std::string value)
            {
                ON_CALL(this->mockUserSettingProvider, KeyExists(_)).WillByDefault(Return(true));

                ON_CALL(this->mockUserSettingProvider, KeyExists(key)).WillByDefault(Return(false));

                EXPECT_CALL(this->mockUserSettingProvider, SetKey(key, description, value)).Times(1);
            }

            void SetExpectationNoLoad(std::string key, std::string description, std::string value)
            {
                ON_CALL(this->mockUserSettingProvider, KeyExists(_)).WillByDefault(Return(true));

                EXPECT_CALL(this->mockUserSettingProvider, SetKey(key, description, value)).Times(0);
            }

            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UserSetting userSetting;
        };

        TEST_F(LoadDefaultUserSettingsTest, ItSetsDefaultValueForPrenotes)
        {
            this->SetExpectation(
                GeneralSettingsEntries::usePrenoteSettingsKey,
                GeneralSettingsEntries::usePrenoteSettingsDescription,
                "0");

            LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(LoadDefaultUserSettingsTest, ItDoesntSetPrenotesIfSettingExists)
        {
            this->SetExpectationNoLoad(
                GeneralSettingsEntries::usePrenoteSettingsKey,
                GeneralSettingsEntries::usePrenoteSettingsDescription,
                "0");

            LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(LoadDefaultUserSettingsTest, ItSetsDefaultValueForSquawks)
        {
            this->SetExpectation(
                GeneralSettingsEntries::squawkToggleSettingsKey,
                GeneralSettingsEntries::squawkToggleSettingsDescription,
                "1");

            LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(LoadDefaultUserSettingsTest, ItDoesntSetSquawksIfSettingExists)
        {
            this->SetExpectationNoLoad(
                GeneralSettingsEntries::squawkToggleSettingsKey,
                GeneralSettingsEntries::squawkToggleSettingsDescription,
                "0");

            LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(LoadDefaultUserSettingsTest, ItSetsDefaultValueForInitialAltitudes)
        {
            this->SetExpectation(
                GeneralSettingsEntries::initialAltitudeToggleSettingsKey,
                GeneralSettingsEntries::initialAltitudeToggleSettingsDescription,
                "1");

            LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(LoadDefaultUserSettingsTest, ItDoesntSetInitialAltitudesIfSettingExists)
        {
            this->SetExpectationNoLoad(
                GeneralSettingsEntries::initialAltitudeToggleSettingsKey,
                GeneralSettingsEntries::initialAltitudeToggleSettingsDescription,
                "0");

            LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(LoadDefaultUserSettingsTest, ItSetsDefaultValueForPressureNotifications)
        {
            this->SetExpectation(
                GeneralSettingsEntries::pressureMonitorSendMessageKey,
                GeneralSettingsEntries::pressureMonitorSendMessageDescription,
                "1");

            LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(LoadDefaultUserSettingsTest, ItDoesntSetPressureNotificationsIfSettingExists)
        {
            this->SetExpectationNoLoad(
                GeneralSettingsEntries::pressureMonitorSendMessageKey,
                GeneralSettingsEntries::pressureMonitorSendMessageDescription,
                "0");

            LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(LoadDefaultUserSettingsTest, ItSetsDefaultValueForTimeFormat)
        {
            this->SetExpectation(
                GeneralSettingsEntries::unknownTimeFormatBlankKey,
                GeneralSettingsEntries::unknownTimeFormatBlankDescription,
                "0");

            LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(LoadDefaultUserSettingsTest, ItDoesntSetTimeFormatIfSettingExists)
        {
            this->SetExpectationNoLoad(
                GeneralSettingsEntries::unknownTimeFormatBlankKey,
                GeneralSettingsEntries::unknownTimeFormatBlankDescription,
                "0");

            LoadDefaultUserSettings(this->userSetting);
        }
    } // namespace Euroscope
} // namespace UKControllerPluginTest
