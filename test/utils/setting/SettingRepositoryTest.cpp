#include "setting/SettingRepository.h"
#include "setting/SettingValue.h"

using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Throw;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Setting::SettingValue;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginUtilsTest {
    namespace Setting {

        TEST(SettingRepository, AddSettingValueAddsSetting)
        {
            StrictMock<MockWinApi> winApiMock;
            SettingRepository repo(winApiMock);
            repo.AddSettingValue({"foo", "bar", "baz"});

            EXPECT_EQ(1, repo.SettingsCount());
        }

        TEST(SettingRepository, AddSettingValueDoesntOverwrite)
        {
            StrictMock<MockWinApi> winApiMock;
            SettingRepository repo(winApiMock);
            repo.AddSettingValue({"foo", "bar", "baz"});
            repo.AddSettingValue({"foo", "bob", "barrie"});

            EXPECT_EQ("bar", repo.GetSetting("foo"));
            EXPECT_EQ(1, repo.SettingsCount());
        }

        TEST(SettingRepository, AddSettingsFromJsonFileStopsIfFileDoesntExist)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test.json")))
                .Times(1)
                .WillOnce(Return(false));

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test.json");
            EXPECT_EQ(0, repo.SettingsCount());
        }

        TEST(SettingRepository, AddSettingsFromJsonFileStopsIfFileReadError)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test.json"), true))
                .Times(1)
                .WillOnce(Throw(std::ifstream::failure("test")));

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test.json");
            EXPECT_EQ(0, repo.SettingsCount());
        }

        TEST(SettingRepository, AddSettingsFromJsonFileStopsIfInvalidJson)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test.json"), true))
                .Times(1)
                .WillOnce(Return("{{}"));

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test.json");
            EXPECT_EQ(0, repo.SettingsCount());
        }

        TEST(SettingRepository, AddSettingsFromJsonFileStopsIfNotJsonObject)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test.json"), true))
                .Times(1)
                .WillOnce(Return("abcd"));

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test.json");
            EXPECT_EQ(0, repo.SettingsCount());
        }

        TEST(SettingRepository, AddSettingsFromJsonFileAddsSettings)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test.json"), true))
                .Times(1)
                .WillOnce(Return("{\"test1\": \"testValue1\", \"test2\": \"testValue2\"}"));

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test.json");
            EXPECT_EQ(2, repo.SettingsCount());
            EXPECT_TRUE(repo.HasSetting("test1"));
            EXPECT_TRUE(repo.HasSetting("test2"));
            EXPECT_TRUE(repo.GetSetting("test1") == "testValue1");
            EXPECT_TRUE(repo.GetSetting("test2") == "testValue2");
        }

        TEST(SettingRepository, AddSettingsFromJsonFileIgnoresDuplicateSettings)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test1.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test2.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test1.json"), true))
                .Times(1)
                .WillOnce(Return("{\"test1\": \"testValue1\"}"));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test2.json"), true))
                .Times(1)
                .WillOnce(Return("{\"test1\": \"testValue2\"}"));

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test1.json");
            repo.AddSettingsFromJsonFile("test/test2.json");
            EXPECT_TRUE(repo.HasSetting("test1"));
            EXPECT_TRUE(repo.GetSetting("test1") == "testValue1");
        }

        TEST(SettingRepository, GetSettingReturnsEmptyStringByDefaultIfNotSet)
        {
            StrictMock<MockWinApi> winApiMock;

            SettingRepository repo(winApiMock);
            EXPECT_TRUE("" == repo.GetSetting("test"));
        }

        TEST(SettingRepository, GetSettingReturnsDefaultvalueIfProvided)
        {
            StrictMock<MockWinApi> winApiMock;

            SettingRepository repo(winApiMock);
            EXPECT_TRUE("test2" == repo.GetSetting("test", "test2"));
        }

        TEST(SettingRepository, UpdateSettingChangesSettingValue)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test.json"), true))
                .Times(1)
                .WillOnce(Return("{\"test1\": \"testValue1\", \"test2\": \"testValue2\"}"));

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test.json");
            repo.UpdateSetting("test1", "notTestValue1");
            EXPECT_TRUE(repo.HasSetting("test1"));
            EXPECT_TRUE(repo.GetSetting("test1") == "notTestValue1");
        }

        TEST(SettingRepository, UpdateSettingDoesNothingIfSettingDoesNotExist)
        {
            StrictMock<MockWinApi> winApiMock;
            SettingRepository repo(winApiMock);
            repo.UpdateSetting("test1", "notTestValue1");
            EXPECT_FALSE(repo.HasSetting("test2"));
        }

        TEST(SettingRepository, WriteSettingsToFileHandlesSingleFile)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test.json"), true))
                .Times(1)
                .WillOnce(Return("{\"test1\": \"testValue1\", \"test2\": \"testValue2\"}"));

            EXPECT_CALL(
                winApiMock,
                WriteToFile(
                    std::wstring(L"settings/test/test.json"),
                    "{\n    \"test1\": \"testValue1\",\n    \"test2\": \"testValue2\"\n}",
                    true,
                    false))
                .Times(1);

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test.json");
            repo.WriteSettingsToFile();
        }

        TEST(SettingRepository, WriteSettingsToFileHandlesUpdatedSettings)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test.json"), true))
                .Times(1)
                .WillOnce(Return("{\"test1\": \"testValue1\", \"test2\": \"testValue2\"}"));

            EXPECT_CALL(
                winApiMock,
                WriteToFile(
                    std::wstring(L"settings/test/test.json"),
                    "{\n    \"test1\": \"notTestValue1\",\n    \"test2\": \"testValue2\"\n}",
                    true,
                    false))
                .Times(1);

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test.json");
            repo.UpdateSetting("test1", "notTestValue1");
            repo.WriteSettingsToFile();
        }

        TEST(SettingRepository, WriteSettingsToFileHandlesMultipleFiles)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test1.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, FileExists(std::wstring(L"settings/test/test2.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test1.json"), true))
                .Times(1)
                .WillOnce(Return("{\"test1\": \"testValue1\"}"));

            EXPECT_CALL(winApiMock, ReadFromFileMock(std::wstring(L"settings/test/test2.json"), true))
                .Times(1)
                .WillOnce(Return("{\"test2\": \"testValue2\"}"));

            EXPECT_CALL(
                winApiMock,
                WriteToFile(
                    std::wstring(L"settings/test/test1.json"), "{\n    \"test1\": \"testValue1\"\n}", true, false))
                .Times(1);

            EXPECT_CALL(
                winApiMock,
                WriteToFile(
                    std::wstring(L"settings/test/test2.json"), "{\n    \"test2\": \"testValue2\"\n}", true, false))
                .Times(1);

            SettingRepository repo(winApiMock);
            repo.AddSettingsFromJsonFile("test/test1.json");
            repo.AddSettingsFromJsonFile("test/test2.json");
            repo.WriteSettingsToFile();
        }
    } // namespace Setting
} // namespace UKControllerPluginUtilsTest
