#include "datablock/DisplayTime.h"
#include "helper/HelperFunctions.h"
#include "euroscope/UserSetting.h"
#include "euroscope/GeneralSettingsEntries.h"

using testing::NiceMock;
using testing::Return;
using testing::Test;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Datablock::DisplayTime;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest {
    namespace Datablock {

        class DisplayTimeTest : public Test
        {
            public:
            DisplayTimeTest() : userSetting(userSettingProvider)
            {
            }
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting;
            DisplayTime timeDisplay;
        };

        TEST_F(DisplayTimeTest, FromTimestampReturnsTimeString)
        {
            std::string test = timeDisplay.FromTimestamp(1403549100);
            EXPECT_TRUE("18:45" == timeDisplay.FromTimestamp(1403549100));
        }

        TEST_F(DisplayTimeTest, FromSystemTimeReturnsTimeString)
        {
            std::string expectedTime = fmt::format("{:%H:%M}", fmt::gmtime(std::chrono::system_clock::now()));
            EXPECT_TRUE(expectedTime == timeDisplay.FromSystemTime());
        }

        TEST_F(DisplayTimeTest, FromTimePointReturnsTimeString)
        {
            std::string expectedTime = "15:23";
            EXPECT_TRUE(expectedTime == timeDisplay.FromTimePoint(HelperFunctions::GetTimeFromNumberString("1523")));
        }

        TEST_F(DisplayTimeTest, UnknownTimeDefaultsToDashes)
        {
            EXPECT_EQ(this->timeDisplay.unknownTimeFormatDefault, this->timeDisplay.GetUnknownTimeFormat());
        }

        TEST_F(DisplayTimeTest, ItLoadsUnknownTimeFormatFromSettings)
        {
            ON_CALL(this->userSettingProvider, GetKey(GeneralSettingsEntries::unknownTimeFormatBlankKey))
                .WillByDefault(Return("1"));

            this->timeDisplay.UserSettingsUpdated(this->userSetting);
            EXPECT_EQ(this->timeDisplay.unknownTimeFormatBlank, this->timeDisplay.GetUnknownTimeFormat());
        }

        TEST_F(DisplayTimeTest, ItDefaultsToDashesIfNoUserSetting)
        {
            ON_CALL(this->userSettingProvider, GetKey(GeneralSettingsEntries::unknownTimeFormatBlankKey))
                .WillByDefault(Return(""));

            this->timeDisplay.UserSettingsUpdated(this->userSetting);
            EXPECT_EQ(this->timeDisplay.unknownTimeFormatDefault, this->timeDisplay.GetUnknownTimeFormat());
        }
    } // namespace Datablock
} // namespace UKControllerPluginTest
