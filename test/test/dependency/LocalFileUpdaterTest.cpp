#include "pch/pch.h"
#include "dependency/LocalFileUpdater.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "api/RemoteFileManifest.h"
#include "api/RemoteFile.h"
#include "api/ApiInterface.h"
#include "api/ApiException.h"
#include "api/ApiNotFoundException.h"

using UKControllerPlugin::Dependency::LocalFileUpdater;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Api::RemoteFileManifest;
using UKControllerPlugin::Api::RemoteFile;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiNotFoundException;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;

namespace UKControllerPluginTest {
    namespace Dependency {

        class LocalFileUpdaterTest : public Test
        {
            public:
                LocalFileUpdaterTest()
                    : updater(this->mockWinApi, this->mockWebApi)
                {

                }

                LocalFileUpdater updater;
                NiceMock<MockWinApi> mockWinApi;
                NiceMock<MockApiInterface> mockWebApi;
        };

        TEST_F(LocalFileUpdaterTest, UpdateLocalFilesFromManifestStopsImmediatelyIfFolderNotCreated)
        {
            EXPECT_CALL(this->mockWinApi, CreateFolder("dependencies"))
                .Times(1)
                .WillOnce(Return(false));

            // Create the local classes for the test;
            RemoteFileManifest manifest;
            this->updater.UpdateLocalFilesFromManifest(manifest);
        }

        TEST_F(LocalFileUpdaterTest, UpdateLocalFilesDownloadsFileFromRemoteIfNotExists)
        {
            // Mock the external APIs
            EXPECT_CALL(this->mockWinApi, CreateFolder("dependencies"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists("dependencies/test.json"))
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(this->mockWinApi, FileExists(LocalFileUpdater::HASH_FILE))
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(
                    this->mockWinApi,
                    WriteToFile(LocalFileUpdater::HASH_FILE, "{\n    \"test.json\": \"testmd5\"\n}", true)
                )
                .Times(1);

            EXPECT_CALL(this->mockWinApi, WriteToFile("dependencies/test.json", "sometestdata", true))
                .Times(1);

            EXPECT_CALL(this->mockWebApi, FetchRemoteFile("testuri"))
                .Times(1)
                .WillOnce(Return("sometestdata"));


            // Create the local classes for the test;
            RemoteFileManifest manifest;
            RemoteFile remoteFile;
            remoteFile.filename = "test.json";
            remoteFile.hash = "testmd5";
            remoteFile.uri = "testuri";
            manifest.AddFile(remoteFile);

            this->updater.UpdateLocalFilesFromManifest(manifest);
        }

        TEST_F(LocalFileUpdaterTest, UpdateLocalFilesDownloadsFileFromRemoteIfHashNoMatch)
        {
            // Mock the external APIs
            EXPECT_CALL(this->mockWinApi, CreateFolder("dependencies"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists("dependencies/test.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists(LocalFileUpdater::HASH_FILE))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, ReadFromFileMock(LocalFileUpdater::HASH_FILE, true))
                .Times(1)
                .WillOnce(Return("{\"test.json\": \"nottestmd5\"}"));

            EXPECT_CALL(
                    this->mockWinApi,
                    WriteToFile(LocalFileUpdater::HASH_FILE, "{\n    \"test.json\": \"testmd5\"\n}", true)
                )
                .Times(1);

            EXPECT_CALL(this->mockWinApi, WriteToFile("dependencies/test.json", "sometestdata", true))
                .Times(1);

            EXPECT_CALL(this->mockWebApi, FetchRemoteFile("testuri"))
                .Times(1)
                .WillOnce(Return("sometestdata"));

            // Create the local classes for the test;
            RemoteFileManifest manifest;
            RemoteFile remoteFile;
            remoteFile.filename = "test.json";
            remoteFile.hash = "testmd5";
            remoteFile.uri = "testuri";
            manifest.AddFile(remoteFile);

            this->updater.UpdateLocalFilesFromManifest(manifest);
        }

        TEST_F(LocalFileUpdaterTest, UpdateLocalFilesDownloadsFileFromRemoteIfHashNotInCache)
        {
            // Mock the external APIs
            EXPECT_CALL(this->mockWinApi, CreateFolder("dependencies"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists("dependencies/test.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists(LocalFileUpdater::HASH_FILE))
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(
                    this->mockWinApi,
                    WriteToFile(LocalFileUpdater::HASH_FILE, "{\n    \"test.json\": \"testmd5\"\n}", true)
                )
                .Times(1);

            EXPECT_CALL(this->mockWinApi, WriteToFile("dependencies/test.json", "sometestdata", true))
                .Times(1);

            EXPECT_CALL(this->mockWebApi, FetchRemoteFile("testuri"))
                .Times(1)
                .WillOnce(Return("sometestdata"));

            // Create the local classes for the test;
            RemoteFileManifest manifest;
            RemoteFile remoteFile;
            remoteFile.filename = "test.json";
            remoteFile.hash = "testmd5";
            remoteFile.uri = "testuri";
            manifest.AddFile(remoteFile);

            this->updater.UpdateLocalFilesFromManifest(manifest);
        }

        TEST_F(LocalFileUpdaterTest, UpdateLocalFilesDoesNotDownloadFileIfUpToDate)
        {
            EXPECT_CALL(this->mockWinApi, CreateFolder("dependencies"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists("dependencies/test.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists(LocalFileUpdater::HASH_FILE))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, ReadFromFileMock(LocalFileUpdater::HASH_FILE, true))
                .Times(1)
                .WillOnce(Return("{\"test.json\": \"testmd5\"}"));

            EXPECT_CALL(
                    this->mockWinApi,
                    WriteToFile(LocalFileUpdater::HASH_FILE, "{\n    \"test.json\": \"testmd5\"\n}", true)
                )
                .Times(1);

            // Create the local classes for the test;
            RemoteFileManifest manifest;
            RemoteFile remoteFile;
            remoteFile.filename = "test.json";
            remoteFile.hash = "testmd5";
            remoteFile.uri = "testuri";
            manifest.AddFile(remoteFile);

            this->updater.UpdateLocalFilesFromManifest(manifest);
        }

        TEST_F(LocalFileUpdaterTest, UpdateLocalFilesDoesNotWriteToHashCacheOnApiException)
        {
            EXPECT_CALL(this->mockWinApi, CreateFolder("dependencies"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists("dependencies/test.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists(LocalFileUpdater::HASH_FILE))
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(this->mockWinApi, WriteToFile(LocalFileUpdater::HASH_FILE, "{}", true))
                .Times(1);

            EXPECT_CALL(this->mockWebApi, FetchRemoteFile("testuri"))
                .Times(1)
                .WillOnce(Throw(ApiException("Not found")));

            // Create the local classes for the test;
            RemoteFileManifest manifest;
            RemoteFile remoteFile;
            remoteFile.filename = "test.json";
            remoteFile.hash = "testmd5";
            remoteFile.uri = "testuri";
            manifest.AddFile(remoteFile);

            this->updater.UpdateLocalFilesFromManifest(manifest);
        }

        TEST_F(LocalFileUpdaterTest, UpdateLocalFilesDoesNotWriteToHashCacheOnHttpNotFound)
        {
            EXPECT_CALL(this->mockWinApi, CreateFolder("dependencies"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists("dependencies/test.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists(LocalFileUpdater::HASH_FILE))
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(this->mockWinApi, WriteToFile(LocalFileUpdater::HASH_FILE, "{}", true))
                .Times(1);

            EXPECT_CALL(this->mockWebApi, FetchRemoteFile("testuri"))
                .Times(1)
                .WillOnce(Throw(ApiNotFoundException("Not found")));

            // Create the local classes for the test;
            RemoteFileManifest manifest;
            RemoteFile remoteFile;
            remoteFile.filename = "test.json";
            remoteFile.hash = "testmd5";
            remoteFile.uri = "testuri";
            manifest.AddFile(remoteFile);

            this->updater.UpdateLocalFilesFromManifest(manifest);
        }

        TEST_F(LocalFileUpdaterTest, UpdateLocalFilesDoesNotWriteToHashCacheOnIoError)
        {
            EXPECT_CALL(this->mockWinApi, CreateFolder("dependencies"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists("dependencies/test.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, FileExists(LocalFileUpdater::HASH_FILE))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->mockWinApi, ReadFromFileMock(LocalFileUpdater::HASH_FILE, true))
                .Times(1)
                .WillOnce(Return("{\"test.json\": \"nottestmd5\"}"));

            EXPECT_CALL(this->mockWinApi, WriteToFile(LocalFileUpdater::HASH_FILE, "{}", true))
                .Times(1);

            EXPECT_CALL(this->mockWinApi, WriteToFile("dependencies/test.json", "sometestdata", true))
                .Times(1)
                .WillOnce(Throw(std::ifstream::failure("")));

            EXPECT_CALL(this->mockWebApi, FetchRemoteFile("testuri"))
                .Times(1)
                .WillOnce(Return("sometestdata"));

            // Create the local classes for the test;
            RemoteFileManifest manifest;
            RemoteFile remoteFile;
            remoteFile.filename = "test.json";
            remoteFile.hash = "testmd5";
            remoteFile.uri = "testuri";
            manifest.AddFile(remoteFile);

            this->updater.UpdateLocalFilesFromManifest(manifest);
        }
    }  // namespace Dependency
}  // namespace UKControllerPluginTest
