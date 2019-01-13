#include "pch/pch.h"
#include "dependency/DependencyCacheFactory.h"
#include "api/RemoteFileManifest.h"
#include "dependency/DependencyCache.h"
#include "mock/MockWinApi.h"
#include "api/RemoteFile.h"

using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Dependency::DependencyCacheFactory;
using UKControllerPlugin::Api::RemoteFileManifest;
using UKControllerPlugin::Api::RemoteFile;
using UKControllerPluginTest::Windows::MockWinApi;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::Throw;

namespace UKControllerPluginTest {
    namespace Dependency {

        TEST(DependencyCacheFactory, ReturnsEmptyCacheIfNoManifest)
        {
            RemoteFileManifest manifest;
            StrictMock<MockWinApi> mockWinApi;

            EXPECT_EQ(0, DependencyCacheFactory::Create("", manifest, mockWinApi)->DependencyCount());
        }

        TEST(DependencyCacheFactory, IgnoresFileIfNotExistsLocally)
        {
            RemoteFileManifest manifest;
            RemoteFile file;
            file.filename = "test.json";
            file.hash = "testhash";
            file.uri = "testuri";
            manifest.AddFile(file);

            StrictMock<MockWinApi> mockWinApi;
            EXPECT_CALL(mockWinApi, FileExists("testfolder/test.json"))
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_EQ(0, DependencyCacheFactory::Create("testfolder", manifest, mockWinApi)->DependencyCount());
        }

        TEST(DependencyCacheFactory, AddsFileIfExistsLocally)
        {
            RemoteFileManifest manifest;
            RemoteFile file;
            file.filename = "test.json";
            file.hash = "testhash";
            file.uri = "testuri";
            manifest.AddFile(file);

            StrictMock<MockWinApi> mockWinApi;
            EXPECT_CALL(mockWinApi, FileExists("testfolder/test.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(mockWinApi, ReadFromFileMock("testfolder/test.json", true))
                .Times(1)
                .WillOnce(Return("testdata"));

            DependencyCache cache = *DependencyCacheFactory::Create("testfolder", manifest, mockWinApi);
            EXPECT_TRUE(cache.HasDependency("test.json"));
            EXPECT_EQ(1, cache.DependencyCount());
        }

        TEST(DependencyCacheFactory, IgnoresFileOnReadFailure)
        {
            RemoteFileManifest manifest;
            RemoteFile file;
            file.filename = "test.json";
            file.hash = "testhash";
            file.uri = "testuri";
            manifest.AddFile(file);

            StrictMock<MockWinApi> mockWinApi;
            EXPECT_CALL(mockWinApi, FileExists("testfolder/test.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(mockWinApi, ReadFromFileMock("testfolder/test.json", true))
                .Times(1)
                .WillOnce(Throw(std::ifstream::failure("test")));

            EXPECT_EQ(0, DependencyCacheFactory::Create("testfolder", manifest, mockWinApi)->DependencyCount());
        }
    }  // namespace Dependency
}  // namespace UKControllerPluginTest
