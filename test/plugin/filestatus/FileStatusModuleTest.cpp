#include "filestatus/FileStatusModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlInterface.h"
#include "curl/CurlResponse.h"
#include "windows/WinApiInterface.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Throw;

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::FileStatus::CheckPackVersion;
using UKControllerPlugin::FileStatus::CheckSectorFileProviderFile;
using UKControllerPlugin::FileStatus::FetchPackVersion;

namespace UKControllerPluginTest {
    namespace FileStatus {

        class MockCurlInterface : public UKControllerPlugin::Curl::CurlInterface
        {
            public:
            MOCK_METHOD(
                UKControllerPlugin::Curl::CurlResponse,
                MakeCurlRequest,
                (const UKControllerPlugin::Curl::CurlRequest& request),
                (override));
        };

        class MockWindowsInterface : public UKControllerPlugin::Windows::WinApiInterface
        {
            public:
            MOCK_METHOD(void, OpenMessageBox, (const wchar_t*, const wchar_t*, unsigned int), (override));
        };

        class FileStatusModuleTest : public ::testing::Test
        {
            protected:
            void SetUp() override
            {
                container.curl = std::make_shared<MockCurlInterface>();
                container.windows = std::make_shared<MockWindowsInterface>();
            }

            PersistenceContainer container;
        };

        // --- FetchPackVersion tests -----------------------------------------------------

        TEST_F(FileStatusModuleTest, FetchPackVersionReturnsTrimmedResponse)
        {
            auto& curl = *container.curl;
            EXPECT_CALL(static_cast<MockCurlInterface&>(curl), MakeCurlRequest(_))
                .WillOnce(Return(UKControllerPlugin::Curl::CurlResponse("  abc123  \n", 200)));

            std::string version = FetchPackVersion(curl);
            EXPECT_EQ(version, "abc123");
        }

        TEST_F(FileStatusModuleTest, FetchPackVersionLogsErrorOnHttpFailure)
        {
            auto& curl = *container.curl;
            EXPECT_CALL(static_cast<MockCurlInterface&>(curl), MakeCurlRequest(_))
                .WillOnce(Return(UKControllerPlugin::Curl::CurlResponse("Not Found", 404)));

            std::string version = FetchPackVersion(curl);
            EXPECT_EQ(version, "");
        }

        TEST_F(FileStatusModuleTest, FetchPackVersionHandlesException)
        {
            auto& curl = *container.curl;
            EXPECT_CALL(static_cast<MockCurlInterface&>(curl), MakeCurlRequest(_))
                .WillOnce(Throw(std::runtime_error("network error")));

            std::string version = FetchPackVersion(curl);
            EXPECT_EQ(version, "");
        }

        // --- CheckPackVersion tests -----------------------------------------------------

        TEST_F(FileStatusModuleTest, CheckPackVersionOpensMessageBoxWhenFileMissing)
        {
            std::filesystem::remove("./UK/Data/Sector/pack_version.txt");

            EXPECT_CALL(*container.windows, OpenMessageBox(_, _, _)).Times(1);
            CheckPackVersion(container);
        }

        TEST_F(FileStatusModuleTest, CheckPackVersionShowsWarningWhenVersionsDiffer)
        {
            std::filesystem::create_directories("./UK/Data/Sector/");
            std::ofstream("./UK/Data/Sector/pack_version.txt") << "local123";

            EXPECT_CALL(*container.curl, MakeCurlRequest(_))
                .WillOnce(Return(UKControllerPlugin::Curl::CurlResponse("remote456", 200)));

            EXPECT_CALL(*container.windows, OpenMessageBox(_, _, _)).Times(1);

            CheckPackVersion(container);
        }

        TEST_F(FileStatusModuleTest, CheckPackVersionDoesNothingWhenVersionsMatch)
        {
            std::filesystem::create_directories("./UK/Data/Sector/");
            std::ofstream("./UK/Data/Sector/pack_version.txt") << "sameversion";

            EXPECT_CALL(*container.curl, MakeCurlRequest(_))
                .WillOnce(Return(UKControllerPlugin::Curl::CurlResponse("sameversion", 200)));

            EXPECT_CALL(*container.windows, OpenMessageBox(_, _, _)).Times(0);

            CheckPackVersion(container);
        }

        // --- CheckSectorFileProviderFile tests ------------------------------------------

        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileDetectsOutdatedUrl)
        {
            std::filesystem::create_directories("./UK/Data/Sector/");
            std::ofstream("./UK/Data/Sector/VATUK_SectorFileProviderDescriptor.txt") << "URL:http://old.link.com";

            EXPECT_CALL(*container.windows, OpenMessageBox(_, _, _)).Times(1);

            CheckSectorFileProviderFile(container);
        }

        TEST_F(FileStatusModuleTest, CheckSectorFileProviderFileNoWarningWhenUrlCorrect)
        {
            std::filesystem::create_directories("./UK/Data/Sector/");
            std::ofstream("./UK/Data/Sector/VATUK_SectorFileProviderDescriptor.txt")
                << "URL:http://docs.vatsim.uk/General/Software%20Downloads/Files/VATUK_Euroscope_files.txt";

            EXPECT_CALL(*container.windows, OpenMessageBox(_, _, _)).Times(0);

            CheckSectorFileProviderFile(container);
        }

    } // namespace FileStatus
} // namespace UKControllerPluginTest
