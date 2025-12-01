#include "filestatus/FileStatusModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"

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

    } // namespace FileStatus
} // namespace UKControllerPluginTest