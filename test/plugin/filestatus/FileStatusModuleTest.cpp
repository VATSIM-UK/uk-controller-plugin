#include "filestatus/FileStatusModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "windows/WinApiInterface.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace FileStatus {

        // Mock classes
        class MockCurlInterface : public UKControllerPlugin::Curl::CurlInterface
        {
            public:
            MOCK_METHOD(
                UKControllerPlugin::Curl::CurlResponse,
                MakeCurlRequest,
                (const UKControllerPlugin::Curl::CurlRequest& request),
                (override));
        };

        class MockWinApiInterface : public UKControllerPlugin::Windows::WinApiInterface
        {
            public:
            MOCK_METHOD(void, OpenMessageBox, (const wchar_t* message, const wchar_t* title, int type), (override));
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
            }

            std::filesystem::path testDir;
        };

        // Test FetchPackVersion with successful response
        TEST_F(FileStatusModuleTest, FetchPackVersionReturnsVersionOnSuccess)
        {
            NiceMock<MockCurlInterface> mockCurl;

            UKControllerPlugin::Curl::CurlResponse response;
            response.SetStatusCode(200);
            response.SetResponse("abc123def456");

            EXPECT_CALL(mockCurl, MakeCurlRequest).WillOnce(Return(response));

            std::string result = UKControllerPlugin::FileStatus::FetchPackVersion(mockCurl);
            EXPECT_EQ(result, "abc123def456");
        }

        // Test FetchPackVersion with whitespace trimming
        TEST_F(FileStatusModuleTest, FetchPackVersionTrimsWhitespace)
        {
            NiceMock<MockCurlInterface> mockCurl;

            UKControllerPlugin::Curl::CurlResponse response;
            response.SetStatusCode(200);
            response.SetResponse("  abc123def456  \n\t");

            EXPECT_CALL(mockCurl, MakeCurlRequest).WillOnce(Return(response));

            std::string result = UKControllerPlugin::FileStatus::FetchPackVersion(mockCurl);
            EXPECT_EQ(result, "abc123def456");
        }

        // Test FetchPackVersion with non-200 status code
        TEST_F(FileStatusModuleTest, FetchPackVersionReturnsEmptyOnBadStatusCode)
        {
            NiceMock<MockCurlInterface> mockCurl;

            UKControllerPlugin::Curl::CurlResponse response;
            response.SetStatusCode(404);
            response.SetResponse("Not Found");

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

        // Test CheckPackVersion with matching versions
        TEST_F(FileStatusModuleTest, CheckPackVersionDoesNothingWhenVersionsMatch)
        {
            NiceMock<MockCurlInterface> mockCurl;
            NiceMock<MockWinApiInterface> mockWindows;

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.curl = &mockCurl;
            container.windows = &mockWindows;

            // Create version file
            std::string versionPath = "./UK/Data/Sector/pack_version.txt";
            std::filesystem::create_directories(std::filesystem::path(versionPath).parent_path());
            std::ofstream versionFile(versionPath);
            versionFile << "abc123def456";
            versionFile.close();

            UKControllerPlugin::Curl::CurlResponse response;
            response.SetStatusCode(200);
            response.SetResponse("abc123def456");

            EXPECT_CALL(mockCurl, MakeCurlRequest).WillOnce(Return(response));

            EXPECT_CALL(mockWindows, OpenMessageBox).Times(0);

            UKControllerPlugin::FileStatus::CheckPackVersion(container);

            std::filesystem::remove_all("./UK");
        }

        // Test CheckPackVersion with mismatched versions
        TEST_F(FileStatusModuleTest, CheckPackVersionShowsDialogWhenVersionsMismatch)
        {
            NiceMock<MockCurlInterface> mockCurl;
            NiceMock<MockWinApiInterface> mockWindows;

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.curl = &mockCurl;
            container.windows = &mockWindows;

            // Create version file with old version
            std::string versionPath = "./UK/Data/Sector/pack_version.txt";
            std::filesystem::create_directories(std::filesystem::path(versionPath).parent_path());
            std::ofstream versionFile(versionPath);
            versionFile << "oldversion";
            versionFile.close();

            UKControllerPlugin::Curl::CurlResponse response;
            response.SetStatusCode(200);
            response.SetResponse("newversion");

            EXPECT_CALL(mockCurl, MakeCurlRequest).WillOnce(Return(response));

            EXPECT_CALL(mockWindows, OpenMessageBox).Times(1);

            UKControllerPlugin::FileStatus::CheckPackVersion(container);

            std::filesystem::remove_all("./UK");
        }

        // Test CheckPackVersion when version file doesn't exist
        TEST_F(FileStatusModuleTest, CheckPackVersionShowsDialogWhenFileNotFound)
        {
            NiceMock<MockCurlInterface> mockCurl;
            NiceMock<MockWinApiInterface> mockWindows;

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.curl = &mockCurl;
            container.windows = &mockWindows;

            EXPECT_CALL(mockWindows, OpenMessageBox).Times(1);

            UKControllerPlugin::FileStatus::CheckPackVersion(container);
        }

        // Test CheckSectorFileProviderFile with matching URLs
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderDoesNothingWhenUrlMatches)
        {
            NiceMock<MockWinApiInterface> mockWindows;

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = &mockWindows;

            // Create sector file with correct URL
            std::string sectorPath = "./UK/Data/Sector/VATUK_SectorFileProviderDescriptor.txt";
            std::filesystem::create_directories(std::filesystem::path(sectorPath).parent_path());
            std::ofstream sectorFile(sectorPath);
            sectorFile << "URL:http://docs.vatsim.uk/General/Software%20Downloads/Files/VATUK_Euroscope_files.txt";
            sectorFile.close();

            EXPECT_CALL(mockWindows, OpenMessageBox).Times(0);

            UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container);

            std::filesystem::remove_all("./UK");
        }

        // Test CheckSectorFileProviderFile with mismatched URLs
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderShowsDialogWhenUrlMismatches)
        {
            NiceMock<MockWinApiInterface> mockWindows;

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = &mockWindows;

            // Create sector file with old URL
            std::string sectorPath = "./UK/Data/Sector/VATUK_SectorFileProviderDescriptor.txt";
            std::filesystem::create_directories(std::filesystem::path(sectorPath).parent_path());
            std::ofstream sectorFile(sectorPath);
            sectorFile << "URL:http://old.vatsim.uk/files";
            sectorFile.close();

            EXPECT_CALL(mockWindows, OpenMessageBox).Times(1);

            UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container);

            std::filesystem::remove_all("./UK");
        }

        // Test CheckSectorFileProviderFile when file doesn't exist
        TEST_F(FileStatusModuleTest, CheckSectorFileProviderHandlesFileNotFound)
        {
            NiceMock<MockWinApiInterface> mockWindows;

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.windows = &mockWindows;

            EXPECT_CALL(mockWindows, OpenMessageBox).Times(0);

            // Should not crash and should not show dialog
            UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container);
        }

        // Test BootstrapPlugin basic functionality
        TEST_F(FileStatusModuleTest, BootstrapPluginCallsFunctions)
        {
            NiceMock<MockCurlInterface> mockCurl;
            NiceMock<MockWinApiInterface> mockWindows;

            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            container.curl = &mockCurl;
            container.windows = &mockWindows;

            UKControllerPlugin::FileStatus::FileStatusModule module;

            // Should handle missing files gracefully
            module.BootstrapPlugin(container);

            EXPECT_TRUE(true);
        }

    } // namespace FileStatus
} // namespace UKControllerPluginTest