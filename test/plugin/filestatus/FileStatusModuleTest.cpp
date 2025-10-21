#include "bootstrap/PersistenceContainer.h"
#include "filestatus/FileStatusModule.h"
#include "mock/MockCurlApi.h"
#include "mock/MockWinApi.h"
#include "test/BootstrapProviderTestCase.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <filesystem>

using testing::NiceMock;
using testing::Return;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::FileStatus {

    // Paths used by FileStatusModule.cpp
    static const std::string PACK_VERSION_PATH = "./UK/Data/Sector/pack_version.txt";
    static const std::string SECTOR_FILE_DOWNLOADER_PATH = "./UK/Data/Sector/VATUK_SectorFileProviderDescriptor.txt";

    static void EnsureParentFolders(const std::string& path)
    {
        std::filesystem::path p(path);
        if (p.has_parent_path()) {
            std::filesystem::create_directories(p.parent_path());
        }
    }

    TEST(FileStatusModule, FetchPackVersion_TrimsWhitespaceAndReturnsBody)
    {
        NiceMock<MockCurlApi> mockCurl;
        EXPECT_CALL(mockCurl, MakeCurlRequest(testing::_))
            .WillOnce(Return(UKControllerPlugin::Curl::CurlResponse("  abc123\n", false, 200)));

        auto result = UKControllerPlugin::FileStatus::FetchPackVersion(mockCurl);
        EXPECT_EQ("abc123", result);
    }

    TEST(FileStatusModule, CheckPackVersion_NoMessageWhenUpToDate)
    {
        // Arrange
        UKControllerPluginTest::BootstrapProviderTestCase helper;
        PersistenceContainer container = helper.MakeContainer();

        auto winApi = std::make_unique<NiceMock<MockWinApi>>();
        auto curl = std::make_unique<NiceMock<MockCurlApi>>();
        container.windows = std::move(winApi);
        container.curl = std::move(curl);

        // Prepare local file with matching content
        EnsureParentFolders(PACK_VERSION_PATH);
        std::ofstream ofs(PACK_VERSION_PATH);
        ofs << "version-xyz";
        ofs.close();

        EXPECT_CALL(*static_cast<MockCurlApi*>(container.curl.get()), MakeCurlRequest(testing::_))
            .WillOnce(Return(UKControllerPlugin::Curl::CurlResponse("version-xyz", false, 200)));

        // Ensure OpenMessageBox is NOT called
        EXPECT_CALL(
            *static_cast<MockWinApi*>(container.windows.get()), OpenMessageBox(testing::_, testing::_, testing::_))
            .Times(0);

        // Act
        UKControllerPlugin::FileStatus::CheckPackVersion(container);

        // Clean up
        std::filesystem::remove(PACK_VERSION_PATH);
    }

    TEST(FileStatusModule, CheckPackVersion_ShowsMessageWhenOutOfDate)
    {
        UKControllerPluginTest::BootstrapProviderTestCase helper;
        PersistenceContainer container = helper.MakeContainer();

        auto winApi = std::make_unique<NiceMock<MockWinApi>>();
        auto curl = std::make_unique<NiceMock<MockCurlApi>>();
        container.windows = std::move(winApi);
        container.curl = std::move(curl);

        EnsureParentFolders(PACK_VERSION_PATH);
        std::ofstream ofs(PACK_VERSION_PATH);
        ofs << "local-version";
        ofs.close();

        EXPECT_CALL(*static_cast<MockCurlApi*>(container.curl.get()), MakeCurlRequest(testing::_))
            .WillOnce(Return(UKControllerPlugin::Curl::CurlResponse("remote-version", false, 200)));

        // Expect a single message box call when out of date
        EXPECT_CALL(
            *static_cast<MockWinApi*>(container.windows.get()), OpenMessageBox(testing::_, testing::_, testing::_))
            .Times(1)
            .WillOnce(Return(1));

        UKControllerPlugin::FileStatus::CheckPackVersion(container);

        std::filesystem::remove(PACK_VERSION_PATH);
    }

    TEST(FileStatusModule, CheckSectorFileProviderFile_NoMessageWhenUpToDate)
    {
        UKControllerPluginTest::BootstrapProviderTestCase helper;
        PersistenceContainer container = helper.MakeContainer();

        auto winApi = std::make_unique<NiceMock<MockWinApi>>();
        container.windows = std::move(winApi);

        EnsureParentFolders(SECTOR_FILE_DOWNLOADER_PATH);
        std::ofstream ofs(SECTOR_FILE_DOWNLOADER_PATH);
        ofs << "URL:http://docs.vatsim.uk/General/Software%20Downloads/Files/VATUK_Euroscope_files.txt\n";
        ofs.close();

        // No message box expected
        EXPECT_CALL(
            *static_cast<MockWinApi*>(container.windows.get()), OpenMessageBox(testing::_, testing::_, testing::_))
            .Times(0);

        UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container);

        std::filesystem::remove(SECTOR_FILE_DOWNLOADER_PATH);
    }

    TEST(FileStatusModule, CheckSectorFileProviderFile_ShowsMessageWhenOutOfDate)
    {
        UKControllerPluginTest::BootstrapProviderTestCase helper;
        PersistenceContainer container = helper.MakeContainer();

        auto winApi = std::make_unique<NiceMock<MockWinApi>>();
        container.windows = std::move(winApi);

        EnsureParentFolders(SECTOR_FILE_DOWNLOADER_PATH);
        std::ofstream ofs(SECTOR_FILE_DOWNLOADER_PATH);
        ofs << "URL:http://old.example.com/something.txt\n";
        ofs.close();

        EXPECT_CALL(
            *static_cast<MockWinApi*>(container.windows.get()), OpenMessageBox(testing::_, testing::_, testing::_))
            .Times(1)
            .WillOnce(Return(1));

        UKControllerPlugin::FileStatus::CheckSectorFileProviderFile(container);

        std::filesystem::remove(SECTOR_FILE_DOWNLOADER_PATH);
    }

} // namespace UKControllerPluginTest::FileStatus
