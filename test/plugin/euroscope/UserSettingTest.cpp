#include "euroscope/UserSetting.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class UserSettingTest : public Test
        {
            public:
            UserSettingTest() : userSetting(mockProvider)
            {
            }

            NiceMock<MockUserSettingProviderInterface> mockProvider;
            UserSetting userSetting;
        };

        TEST_F(UserSettingTest, HasEntryReturnsFalseIfKeyNotExists)
        {
            ON_CALL(this->mockProvider, KeyExists("testkey")).WillByDefault(Return(false));

            EXPECT_FALSE(this->userSetting.HasEntry("testkey"));
        }

        TEST_F(UserSettingTest, HasEntryReturnsTrueIfKeyExists)
        {
            ON_CALL(this->mockProvider, KeyExists("testkey")).WillByDefault(Return(true));

            EXPECT_TRUE(this->userSetting.HasEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetBooleanEntryReturnsFalseForStringZero)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("0"));

            EXPECT_FALSE(this->userSetting.GetBooleanEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetBooleanEntryReturnsTrueForStringOne)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("1"));

            EXPECT_TRUE(this->userSetting.GetBooleanEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetBooleanEntryReturnsFalseByDefault)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return(""));

            EXPECT_FALSE(this->userSetting.GetBooleanEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetBooleanEntryReturnsProvidedDefaultValueIfNoSetting)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return(""));

            EXPECT_TRUE(this->userSetting.GetBooleanEntry("testkey", true));
        }

        TEST_F(UserSettingTest, GetColourEntryReturnsColourIfExistsAndValid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("1,2,3"));

            EXPECT_EQ(RGB(1, 2, 3), userSetting.GetColourEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetColourEntryReturnsWhiteIfNotExists)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return(""));

            EXPECT_EQ(RGB(255, 255, 255), userSetting.GetColourEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetColourEntryReturnsProvidedDefaultIfNoValueExists)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return(""));

            EXPECT_EQ(RGB(55, 66, 77), userSetting.GetColourEntry("testkey", RGB(55, 66, 77)));
        }

        TEST_F(UserSettingTest, GetColourEntryReturnsWhiteIfInvalidColour)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("12,3"));

            EXPECT_EQ(RGB(255, 255, 255), userSetting.GetColourEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetFloatEntryReturnsDoubleIfExistsAndIsValid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("1.232"));

            EXPECT_EQ(1.232, userSetting.GetFloatEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetFloatEntryReturnsZeroIfInvalid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("12,3"));

            EXPECT_EQ(0.0, userSetting.GetFloatEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetFloatEntryReturnsProvidedDefaultIfInvalid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return(""));

            EXPECT_EQ(1.25, userSetting.GetFloatEntry("testkey", 1.25));
        }

        TEST_F(UserSettingTest, GetIntegerEntryReturnsIntegerIfExistsAndIsValid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("123"));

            EXPECT_EQ(123, userSetting.GetIntegerEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetIntegerEntryReturnsZeroIfInvalid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("12,3"));

            EXPECT_EQ(0, userSetting.GetIntegerEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetIntegerEntryProvidedDefaultIfInvalid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("12,3"));

            EXPECT_EQ(42, userSetting.GetIntegerEntry("testkey", 42));
        }

        TEST_F(UserSettingTest, GetUnsignedIntegerEntryReturnsIntegerIfExistsAndIsValid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("123"));

            EXPECT_EQ(123, userSetting.GetUnsignedIntegerEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetUnsignedIntegerEntryReturnsZeroIfInvalid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("12,3"));

            EXPECT_EQ(0, userSetting.GetUnsignedIntegerEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetUnsignedIntegerEntryProvidedDefaultIfInvalid)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("12,3"));

            EXPECT_EQ(42, userSetting.GetUnsignedIntegerEntry("testkey", 42));
        }

        TEST_F(UserSettingTest, GetUnsignedIntegerEntryReturnsDefaultOnNegativeNumbers)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("-65"));

            EXPECT_EQ(66, userSetting.GetUnsignedIntegerEntry("testkey", 66));
        }

        TEST_F(UserSettingTest, GetStringEntryReturnsStringIfExists)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("testvalue"));

            EXPECT_TRUE("testvalue" == userSetting.GetStringEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetStringEntryReturnsEmptyStringIfNotExists)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return(""));

            EXPECT_TRUE("" == userSetting.GetStringEntry("testkey"));
        }

        TEST_F(UserSettingTest, GetStringEntryReturnsProvidedDefaultIfNotExists)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return(""));

            EXPECT_TRUE("defaultValue" == userSetting.GetStringEntry("testkey", "defaultValue"));
        }

        TEST_F(UserSettingTest, GetStringListEntryReturnsDefaultIfNoValue)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return(""));

            std::vector<std::string> expected = {"a", "b", "c"};
            EXPECT_EQ(expected, this->userSetting.GetStringListEntry("testkey", expected));
        }

        TEST_F(UserSettingTest, GetStringListEntryReturnsValues)
        {
            ON_CALL(this->mockProvider, GetKey("testkey")).WillByDefault(Return("a;b;c"));

            std::vector<std::string> expected = {"a", "b", "c"};
            EXPECT_EQ(expected, this->userSetting.GetStringListEntry("testkey", {}));
        }

        TEST_F(UserSettingTest, SaveStringSendsAppropriateData)
        {
            EXPECT_CALL(this->mockProvider, SetKey("testkey", "testdescription", "testvalue")).Times(1);

            EXPECT_NO_THROW(this->userSetting.Save("testkey", "testdescription", std::string("testvalue")));
        }

        TEST_F(UserSettingTest, SaveIntegerSendsAppropriateData)
        {
            EXPECT_CALL(this->mockProvider, SetKey("testkey", "testdescription", "1223")).Times(1);

            EXPECT_NO_THROW(this->userSetting.Save("testkey", "testdescription", 1223));
        }

        TEST_F(UserSettingTest, SaveBoolSendsAppropriateDataTrue)
        {
            EXPECT_CALL(this->mockProvider, SetKey("testkey", "testdescription", "1")).Times(1);

            EXPECT_NO_THROW(this->userSetting.Save("testkey", "testdescription", true));
        }

        TEST_F(UserSettingTest, SaveBoolSendsAppropriateDataFalse)
        {
            EXPECT_CALL(this->mockProvider, SetKey("testkey", "testdescription", "0")).Times(1);

            EXPECT_NO_THROW(this->userSetting.Save("testkey", "testdescription", false));
        }

        TEST_F(UserSettingTest, SaveFloatSendsAppropriateData)
        {
            EXPECT_CALL(this->mockProvider, SetKey("testkey", "testdescription", "1223.455600")).Times(1);

            EXPECT_NO_THROW(this->userSetting.Save("testkey", "testdescription", 1223.4556));
        }

        TEST_F(UserSettingTest, SaveColourSendsAppropriateData)
        {
            EXPECT_CALL(this->mockProvider, SetKey("testkey", "testdescription", "111,222,123")).Times(1);

            EXPECT_NO_THROW(this->userSetting.Save("testkey", "testdescription", RGB(111, 222, 123)));
        }

        TEST_F(UserSettingTest, SaveStringVectorSendsAppropriateData)
        {
            EXPECT_CALL(this->mockProvider, SetKey("testkey", "testdescription", "a;b;c")).Times(1);

            EXPECT_NO_THROW(
                this->userSetting.Save("testkey", "testdescription", std::vector<std::string>({"a", "b", "c"})));
        }
    } // namespace Euroscope
} // namespace UKControllerPluginTest
