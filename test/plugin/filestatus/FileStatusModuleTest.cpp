#include "filestatus/FileStatusModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"
#include "mock/MockWinApi.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace FileStatus {

        // Mock Curl interface
        class MockCurlInterface : public UKControllerPlugin::Curl::CurlInterface
        {
            public:
            MOCK_METHOD(
                UKControllerPlugin::Curl::CurlResponse,
                MakeCurlRequest,
                (const UKControllerPlugin::Curl::CurlRequest& request),
                (override));
        };

        class FileStatusModuleTest : public Test
        {
            protected:
            virtual void SetUp()
            {
                // Create temp directory for test files
                testDir = std::filesystem::temp_directory_path() / "filestatus_test";
                std::filesystem::create_directories(testDir);
            }

            virtual void TearDown()
            {
                // Clean up temp files
                if (std::filesystem::exists(testDir)) {
                    std::filesystem::remove_all(testDir);
                }
                if (std::filesystem::exists("./UK")) {
                    std::filesystem::remove_all("./UK");
                }
            }

            std::filesystem::path testDir;
        };

        // Test FetchPackVersion with successful response
        TEST_F(FileStatusModuleTest, FetchPackVersionReturnsVersionOnSuccess)
        {
            NiceMock<MockCurlInterface> mockCurl;
            UKControllerPlugin::Curl::CurlResponse response("abc123def456", false, 200);

            EXPECT_CALL(mockCurl, MakeCurlRequest).WillOnce(Return(response));

            std::string result = UKControllerPlugin::FileStatus::FetchPackVersion(mockCurl);
            EXPECT_EQ(result, "abc123def456");
        }

        // Test FetchPackVersion with whitespace trimming
        TEST_F(FileStatusModuleTest, FetchPackVersionTrimsWhitespace)
        {
            NiceMock<MockCurlInterface> mockCurl;
            UKControllerPlugin::Curl::CurlResponse response("  abc123def456  \n\t", false, 200);

            EXPECT_CALL(mockCurl, MakeCurlRequest).WillOnce(Return(response));

            std::string result = UKControllerPlugin::FileStatus::FetchPackVersion(mockCurl);
            EXPECT_EQ(result, "abc123def456");
        }

        // Test FetchPackVersion with non-200 status code
        TEST_F(FileStatusModuleTest, FetchPackVersionReturnsEmptyOnBadStatusCode)
        {
            NiceMock<MockCurlInterface> mockCurl;
            UKControllerPlugin::Curl::CurlResponse response("Not Found", false, 404);

            EXPECT_CALL(mockCurl, MakeCurlRequest).WillOnce(Return(response));

            std::string result = UKControllerPlugin::FileStatus::FetchPackVersion(mockCurl);
            EXPECT_EQ(result, "");
        }

        // Test FetchPackVersion with exception
        TEST_F(FileStatusModuleTest, FetchPackVersionReturnsEmptyOnException)
        {
            NiceMock<MockCurlInterface> mockCurl;

            EXPECT_CALL(mockCurl, MakeCurlRequest).WillOnce(::testing::Throw(std::runtime_error("Network error")));

            std::string result = UKControllerPlugin::FileStatus::FetchPackVersion(mockCurl);
            EXPECT_EQ(result, "");
        }

        // Tests for CheckPackVersion function

        // Test when local version file does not exist
        TEST_F(FileStatusModuleTest, CheckPackVersionShowsWarningWhenFileNotFound)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto curl = std::make_unique<NiceMock<MockCurlInterface>>();
            auto windowsPtr = windows.get();

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);
            container.curl = std::move(curl);

            // Ensure the file doesn't exist
            std::filesystem::path versionFile("./UK/Data/Sector/pack_version.txt");
            if (std::filesystem::exists(versionFile)) {
                std::filesystem::remove_all("./UK");
            }

            // Should show warning message box when file not found
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(windowsPtr),
                OpenMessageBox(
                    ::testing::StrEq(L"You must update your controller pack"),
                    ::testing::StrEq(L"Out of Date Controller Pack Detected"),
                    ::testing::_))
                .Times(1)
                .WillOnce(Return(0));

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckPackVersion(container));
        }

        // Test when local version file cannot be opened
        TEST_F(FileStatusModuleTest, CheckPackVersionHandlesFileOpenFailure)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto curl = std::make_unique<NiceMock<MockCurlInterface>>();

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);
            container.curl = std::move(curl);

            // Create the directory and file
            std::filesystem::path versionDir("./UK/Data/Sector");
            std::filesystem::path versionFile = versionDir / "pack_version.txt";
            std::filesystem::create_directories(versionDir);

            std::ofstream file(versionFile);
            file << "abc123def456\n";
            file.close();

            // Remove read permissions to cause open failure
            std::filesystem::permissions(versionFile, std::filesystem::perms::none);

            // Should not throw on open failure
            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckPackVersion(container));

            // Restore permissions for cleanup
            std::filesystem::permissions(
                versionFile,
                std::filesystem::perms::owner_read | std::filesystem::perms::owner_write,
                std::filesystem::perm_options::replace);
        }

        // Test when versions match (local version equals GitHub version)
        TEST_F(FileStatusModuleTest, CheckPackVersionSucceedsWhenVersionsMatch)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto curl = std::make_unique<NiceMock<MockCurlInterface>>();

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);
            container.curl = std::move(curl);

            // Create the directory and file with a version
            std::filesystem::path versionDir("./UK/Data/Sector");
            std::filesystem::path versionFile = versionDir / "pack_version.txt";
            std::filesystem::create_directories(versionDir);

            std::string testVersion = "v1.2.3abc123def456";
            std::ofstream file(versionFile);
            file << testVersion;
            file.close();

            // Mock the curl request to return the same version
            UKControllerPlugin::Curl::CurlResponse response(testVersion, false, 200);
            EXPECT_CALL(*curl, MakeCurlRequest).WillOnce(Return(response));

            // Should not show message box when versions match
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(container.windows.get()),
                OpenMessageBox)
                .Times(0);

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckPackVersion(container));
        }

        // Test when versions don't match (local version differs from GitHub version)
        TEST_F(FileStatusModuleTest, CheckPackVersionShowsWarningWhenVersionsMismatch)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto curl = std::make_unique<NiceMock<MockCurlInterface>>();
            auto windowsPtr = windows.get();

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);
            container.curl = std::move(curl);

            // Create the directory and file with an old version
            std::filesystem::path versionDir("./UK/Data/Sector");
            std::filesystem::path versionFile = versionDir / "pack_version.txt";
            std::filesystem::create_directories(versionDir);

            std::ofstream file(versionFile);
            file << "v1.0.0old123";
            file.close();

            // Mock the curl request to return a different version
            UKControllerPlugin::Curl::CurlResponse response("v2.0.0new456", false, 200);
            EXPECT_CALL(*curl, MakeCurlRequest).WillOnce(Return(response));

            // Should show warning message box when versions don't match
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(windowsPtr),
                OpenMessageBox(
                    ::testing::StrEq(L"You must update your controller pack"),
                    ::testing::StrEq(L"Out of Date Controller Pack Detected"),
                    ::testing::_))
                .Times(1)
                .WillOnce(Return(0));

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckPackVersion(container));
        }

        // Test when FetchPackVersion returns empty string (fetch failed)
        TEST_F(FileStatusModuleTest, CheckPackVersionHandlesEmptyGithubVersion)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto curl = std::make_unique<NiceMock<MockCurlInterface>>();

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);
            container.curl = std::move(curl);

            // Create the directory and file
            std::filesystem::path versionDir("./UK/Data/Sector");
            std::filesystem::path versionFile = versionDir / "pack_version.txt";
            std::filesystem::create_directories(versionDir);

            std::ofstream file(versionFile);
            file << "v1.2.3abc123def456";
            file.close();

            // Mock the curl request to return error response (non-200)
            UKControllerPlugin::Curl::CurlResponse response("Not Found", false, 404);
            EXPECT_CALL(*curl, MakeCurlRequest).WillOnce(Return(response));

            // Should not show message box when fetch fails
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(container.windows.get()),
                OpenMessageBox)
                .Times(0);

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckPackVersion(container));
        }

        // Test version comparison with whitespace in file
        TEST_F(FileStatusModuleTest, CheckPackVersionComparesVersionsWithWhitespace)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto curl = std::make_unique<NiceMock<MockCurlInterface>>();
            auto windowsPtr = windows.get();

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);
            container.curl = std::move(curl);

            // Create the directory and file with version containing leading whitespace
            std::filesystem::path versionDir("./UK/Data/Sector");
            std::filesystem::path versionFile = versionDir / "pack_version.txt";
            std::filesystem::create_directories(versionDir);

            std::ofstream file(versionFile);
            file << "  v1.2.3abc123def456  \n"; // Version with whitespace
            file.close();

            // Mock the curl request to return version without whitespace
            UKControllerPlugin::Curl::CurlResponse response("v1.2.3abc123def456", false, 200);
            EXPECT_CALL(*curl, MakeCurlRequest).WillOnce(Return(response));

            // Should show warning because versions don't exactly match (whitespace difference)
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(windowsPtr),
                OpenMessageBox(
                    ::testing::StrEq(L"You must update your controller pack"),
                    ::testing::StrEq(L"Out of Date Controller Pack Detected"),
                    ::testing::_))
                .Times(1)
                .WillOnce(Return(0));

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckPackVersion(container));
        }

        // Test with empty version file
        TEST_F(FileStatusModuleTest, CheckPackVersionHandlesEmptyVersionFile)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto curl = std::make_unique<NiceMock<MockCurlInterface>>();
            auto windowsPtr = windows.get();

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);
            container.curl = std::move(curl);

            // Create empty version file
            std::filesystem::path versionDir("./UK/Data/Sector");
            std::filesystem::path versionFile = versionDir / "pack_version.txt";
            std::filesystem::create_directories(versionDir);

            std::ofstream file(versionFile);
            file << ""; // Empty file
            file.close();

            // Mock the curl request to return a version
            UKControllerPlugin::Curl::CurlResponse response("v1.2.3abc123def456", false, 200);
            EXPECT_CALL(*curl, MakeCurlRequest).WillOnce(Return(response));

            // Should show warning because empty file doesn't match GitHub version
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(windowsPtr),
                OpenMessageBox(
                    ::testing::StrEq(L"You must update your controller pack"),
                    ::testing::StrEq(L"Out of Date Controller Pack Detected"),
                    ::testing::_))
                .Times(1)
                .WillOnce(Return(0));

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckPackVersion(container));
        }

        // Test exception handling in CheckPackVersion
        TEST_F(FileStatusModuleTest, CheckPackVersionHandlesException)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto curl = std::make_unique<NiceMock<MockCurlInterface>>();

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);
            container.curl = std::move(curl);

            // Create the directory and file
            std::filesystem::path versionDir("./UK/Data/Sector");
            std::filesystem::path versionFile = versionDir / "pack_version.txt";
            std::filesystem::create_directories(versionDir);

            std::ofstream file(versionFile);
            file << "v1.2.3abc123def456";
            file.close();

            // Mock the curl request to throw an exception
            EXPECT_CALL(*curl, MakeCurlRequest).WillOnce(::testing::Throw(std::runtime_error("Network error")));

            // Should not throw when exception occurs during fetch
            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckPackVersion(container));
        }

        // Tests for CheckSectorFileProviderFile function

        // Test when sector file provider file does not exist
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileHandlesFileNotFound)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);

            // Ensure the file doesn't exist
            std::filesystem::path sectorFile("./UK/Data/Sector/VATUK_SectorFileProviderDescriptor.txt");
            if (std::filesystem::exists(sectorFile)) {
                std::filesystem::remove_all("./UK");
            }

            // Should not throw and should log error
            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container));
        }

        // Test when sector file provider file cannot be opened
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileHandlesOpenFailure)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);

            // Create the directory and file with restricted permissions
            std::filesystem::path sectorDir("./UK/Data/Sector");
            std::filesystem::path sectorFile = sectorDir / "VATUK_SectorFileProviderDescriptor.txt";
            std::filesystem::create_directories(sectorDir);

            std::ofstream file(sectorFile);
            file << "URL:http://example.com\n";
            file.close();

            // Remove read permissions to cause open failure
            std::filesystem::permissions(sectorFile, std::filesystem::perms::none);

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container));

            // Restore permissions for cleanup
            std::filesystem::permissions(
                sectorFile,
                std::filesystem::perms::owner_read | std::filesystem::perms::owner_write,
                std::filesystem::perm_options::replace);
        }

        // Test when URL is found and matches expected URL
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileSucceedsWithValidUrl)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);

            // Create the directory and file
            std::filesystem::path sectorDir("./UK/Data/Sector");
            std::filesystem::path sectorFile = sectorDir / "VATUK_SectorFileProviderDescriptor.txt";
            std::filesystem::create_directories(sectorDir);

            std::ofstream file(sectorFile);
            file << "URL:http://docs.vatsim.uk/General/Software%20Downloads/Files/VATUK_Euroscope_files.txt\n";
            file.close();

            // Should not call OpenMessageBox if URL is correct
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(container.windows.get()),
                OpenMessageBox)
                .Times(0);

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container));
        }

        // Test when URL is found but doesn't match expected URL
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileShowsWarningWhenUrlMismatch)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto windowsPtr = windows.get();
            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);

            // Create the directory and file with incorrect URL
            std::filesystem::path sectorDir("./UK/Data/Sector");
            std::filesystem::path sectorFile = sectorDir / "VATUK_SectorFileProviderDescriptor.txt";
            std::filesystem::create_directories(sectorDir);

            std::ofstream file(sectorFile);
            file << "URL:http://old.example.com/outdated_url\n";
            file.close();

            // Should call OpenMessageBox with warning about outdated URL
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(windowsPtr),
                OpenMessageBox(::testing::_, ::testing::StrEq(L"Outdated Sector File Configuration"), ::testing::_))
                .Times(1)
                .WillOnce(Return(0));

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container));
        }

        // Test edge case: line is exactly "URL:" without any content
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileHandlesEmptyUrlField)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto windowsPtr = windows.get();
            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);

            // Create the directory and file with just "URL:" and no content
            std::filesystem::path sectorDir("./UK/Data/Sector");
            std::filesystem::path sectorFile = sectorDir / "VATUK_SectorFileProviderDescriptor.txt";
            std::filesystem::create_directories(sectorDir);

            std::ofstream file(sectorFile);
            file << "URL:\n"; // Just "URL:" without any actual URL
            file.close();

            // Empty URL doesn't match the expected URL, so should show warning
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(windowsPtr),
                OpenMessageBox(::testing::_, ::testing::StrEq(L"Outdated Sector File Configuration"), ::testing::_))
                .Times(1)
                .WillOnce(Return(0));

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container));
        }

        // Test with multiple lines where URL is on second line
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileFindsUrlOnSecondLine)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);

            // Create the directory and file
            std::filesystem::path sectorDir("./UK/Data/Sector");
            std::filesystem::path sectorFile = sectorDir / "VATUK_SectorFileProviderDescriptor.txt";
            std::filesystem::create_directories(sectorDir);

            std::ofstream file(sectorFile);
            file << "Some header line\n";
            file << "URL:http://docs.vatsim.uk/General/Software%20Downloads/Files/VATUK_Euroscope_files.txt\n";
            file << "Some footer line\n";
            file.close();

            // Should find the correct URL on the second line
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(container.windows.get()),
                OpenMessageBox)
                .Times(0);

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container));
        }

        // Test with URL that has leading/trailing whitespace (should be trimmed)
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileTrimsUrlWhitespace)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);

            // Create the directory and file with URL containing leading/trailing whitespace
            std::filesystem::path sectorDir("./UK/Data/Sector");
            std::filesystem::path sectorFile = sectorDir / "VATUK_SectorFileProviderDescriptor.txt";
            std::filesystem::create_directories(sectorDir);

            std::ofstream file(sectorFile);
            file
                << "URL:  http://docs.vatsim.uk/General/Software%20Downloads/Files/VATUK_Euroscope_files.txt  \n"; // URL
                                                                                                                   // with
                                                                                                                   // leading/trailing
                                                                                                                   // spaces
            file.close();

            // Should not call OpenMessageBox because whitespace is trimmed and URL matches
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(container.windows.get()),
                OpenMessageBox)
                .Times(0);

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container));
        }

        // Test when file has no URL line at all
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileHandlesNoUrlLine)
        {
            auto windows = std::make_unique<NiceMock<UKControllerPluginTest::Windows::MockWinApi>>();
            auto windowsPtr = windows.get();
            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = std::move(windows);

            // Create the directory and file without any URL line
            std::filesystem::path sectorDir("./UK/Data/Sector");
            std::filesystem::path sectorFile = sectorDir / "VATUK_SectorFileProviderDescriptor.txt";
            std::filesystem::create_directories(sectorDir);

            std::ofstream file(sectorFile);
            file << "Some header line\n";
            file << "Another line without URL\n";
            file.close();

            // No URL found means empty extractedUrl, which doesn't match expected URL
            EXPECT_CALL(
                *static_cast<NiceMock<UKControllerPluginTest::Windows::MockWinApi>*>(windowsPtr),
                OpenMessageBox(::testing::_, ::testing::StrEq(L"Outdated Sector File Configuration"), ::testing::_))
                .Times(1)
                .WillOnce(Return(0));

            EXPECT_NO_THROW(UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container));
        }

    } // namespace FileStatus
} // namespace UKControllerPluginTest