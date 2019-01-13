#include "pch/pch.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "euroscope/UserSetting.h"

using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Euroscope {

        TEST(UserSetting, GetBooleanEntryReturnsFalseForStringZero)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("0"));

            UserSetting userSetting(mockProvider);

            EXPECT_FALSE(userSetting.GetBooleanEntry("testkey"));
        }

        TEST(UserSetting, GetBooleanEntryReturnsTrueForStringOne)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("1"));

            UserSetting userSetting(mockProvider);

            EXPECT_TRUE(userSetting.GetBooleanEntry("testkey"));
        }

        TEST(UserSetting, GetBooleanEntryReturnsFalseByDefault)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return(""));

            UserSetting userSetting(mockProvider);
            EXPECT_FALSE(userSetting.GetBooleanEntry("testkey"));
        }

        TEST(UserSetting, GetBooleanEntryReturnsProvidedDefaultValueIfNoSetting)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return(""));

            UserSetting userSetting(mockProvider);
            EXPECT_TRUE(userSetting.GetBooleanEntry("testkey", true));
        }

        TEST(UserSetting, GetColourEntryReturnsColourIfExistsAndValid)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("1,2,3"));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(RGB(1, 2, 3), userSetting.GetColourEntry("testkey"));
        }

        TEST(UserSetting, GetColourEntryReturnsWhiteIfNotExists)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return(""));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(RGB(255, 255, 255), userSetting.GetColourEntry("testkey"));
        }

        TEST(UserSetting, GetColourEntryReturnsProvidedDefaultIfNoValueExists)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return(""));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(RGB(55, 66, 77), userSetting.GetColourEntry("testkey", RGB(55, 66, 77)));
        }

        TEST(UserSetting, GetColourEntryReturnsWhiteIfInvalidColour)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;

            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("12,3"));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(RGB(255, 255, 255), userSetting.GetColourEntry("testkey"));
        }

        TEST(UserSetting, GetFloatEntryReturnsDoubleIfExistsAndIsValid)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;

            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("1.232"));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(1.232, userSetting.GetFloatEntry("testkey"));
        }

        TEST(UserSetting, GetFloatEntryReturnsZeroIfInvalid)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;

            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("12,3"));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(0.0, userSetting.GetFloatEntry("testkey"));
        }

        TEST(UserSetting, GetFloatEntryReturnsProvidedDefaultIfInvalid)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;

            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return(""));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(1.25, userSetting.GetFloatEntry("testkey", 1.25));
        }

        TEST(UserSetting, GetIntegerEntryReturnsIntegerIfExistsAndIsValid)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;

            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("123"));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(123, userSetting.GetIntegerEntry("testkey"));
        }

        TEST(UserSetting, GetIntegerEntryReturnsZeroIfInvalid)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;

            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("12,3"));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(0, userSetting.GetIntegerEntry("testkey"));
        }

        TEST(UserSetting, GetIntegerEntryProvidedDefaultIfInvalid)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;

            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("12,3"));

            UserSetting userSetting(mockProvider);
            EXPECT_EQ(42, userSetting.GetIntegerEntry("testkey", 42));
        }

        TEST(UserSetting, GetStringEntryReturnsStringIfExists)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;

            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return("testvalue"));

            UserSetting userSetting(mockProvider);
            EXPECT_TRUE("testvalue" == userSetting.GetStringEntry("testkey"));
        }

        TEST(UserSetting, GetStringEntryReturnsEmptyStringIfNotExists)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return(""));

            UserSetting userSetting(mockProvider);
            EXPECT_TRUE("" == userSetting.GetStringEntry("testkey"));
        }

        TEST(UserSetting, GetStringEntryReturnsProvidedDefaultIfNotExists)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            ON_CALL(mockProvider, GetKey("testkey"))
                .WillByDefault(Return(""));

            UserSetting userSetting(mockProvider);
            EXPECT_TRUE("defaultValue" == userSetting.GetStringEntry("testkey", "defaultValue"));
        }

        TEST(UserSetting, SaveStringSendsAppropriateData)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            EXPECT_CALL(mockProvider, SetKey("testkey", "testdescription", "testvalue"))
                .Times(1);

            UserSetting userSetting(mockProvider);
            EXPECT_NO_THROW(userSetting.Save("testkey", "testdescription", std::string("testvalue")));
        }

        TEST(UserSetting, SaveIntegerSendsAppropriateData)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            EXPECT_CALL(mockProvider, SetKey("testkey", "testdescription", "1223"))
                .Times(1);

            UserSetting userSetting(mockProvider);
            EXPECT_NO_THROW(userSetting.Save("testkey", "testdescription", 1223));
        }

        TEST(UserSetting, SaveBoolSendsAppropriateDataTrue)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            EXPECT_CALL(mockProvider, SetKey("testkey", "testdescription", "1"))
                .Times(1);

            UserSetting userSetting(mockProvider);
            EXPECT_NO_THROW(userSetting.Save("testkey", "testdescription", true));
        }

        TEST(UserSetting, SaveBoolSendsAppropriateDataFalse)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            EXPECT_CALL(mockProvider, SetKey("testkey", "testdescription", "0"))
                .Times(1);

            UserSetting userSetting(mockProvider);
            EXPECT_NO_THROW(userSetting.Save("testkey", "testdescription", false));
        }

        TEST(UserSetting, SaveFloatSendsAppropriateData)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            EXPECT_CALL(mockProvider, SetKey("testkey", "testdescription", "1223.455600"))
                .Times(1);

            UserSetting userSetting(mockProvider);
            EXPECT_NO_THROW(userSetting.Save("testkey", "testdescription", 1223.4556));
        }

        TEST(UserSetting, SaveColourSendsAppropriateData)
        {
            NiceMock<MockUserSettingProviderInterface> mockProvider;
            EXPECT_CALL(mockProvider, SetKey("testkey", "testdescription", "111,222,123"))
                .Times(1);

            UserSetting userSetting(mockProvider);
            EXPECT_NO_THROW(userSetting.Save("testkey", "testdescription", RGB(111, 222, 123)));
        }
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
