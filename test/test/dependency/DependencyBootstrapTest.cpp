#include "pch/pch.h"
#include "dependency/DependencyBootstrap.h"
#include "dependency/DependencyCache.h"
#include "mock/MockApiInterface.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/null_sink.h"
#include "mock/MockWinApi.h"
#include "mock/MockCurlApi.h"
#include "curl/CurlResponse.h"
#include "api/ApiRequestBuilder.h"
#include "curl/CurlRequest.h"
#include "api/RemoteFileManifest.h"
#include "api/RemoteFile.h"

using UKControllerPlugin::Bootstrap::DependencyBootstrap;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Api::RemoteFileManifest;
using UKControllerPlugin::Api::RemoteFile;
using UKControllerPluginTest::Curl::MockCurlApi;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
namespace Bootstrap {

TEST(DependencyBootstrap, BootstrapUpdatesLocalFilesAndProducesADependencyCache)
{
    NiceMock<MockCurlApi> mockCurl;
    NiceMock<MockWinApi> mockWindows;
    NiceMock<MockApiInterface> mockApi;

    RemoteFileManifest manifest;
    RemoteFile file;
    file.filename = "test1.json";
    file.hash = "nottestmd5";
    file.uri = "http://test1.com";
    manifest.AddFile(file);


    // The response we expect to get when asking for the manifest from the API
    EXPECT_CALL(mockApi, FetchDependencyManifest())
        .Times(1)
        .WillOnce(Return(manifest));

    EXPECT_CALL(mockApi, FetchRemoteFile("http://test1.com"))
        .Times(1)
        .WillOnce(Return("somethingnew"));

    // For simulation purposes, we'll just assume that the local file is up to date.
    EXPECT_CALL(mockWindows, CreateFolder("dependencies"))
        .Times(1)
        .WillOnce(Return(true));

    EXPECT_CALL(mockWindows, FileExists("dependencies/test1.json"))
        .Times(2)
        .WillRepeatedly(Return(true));

    EXPECT_CALL(mockWindows, FileExists("dependencies/filehash.json"))
        .Times(1)
        .WillOnce(Return(true));

    EXPECT_CALL(mockWindows, ReadFromFileMock("dependencies/filehash.json", true))
        .Times(1)
        .WillOnce(Return("{\"test1.json\": \"testmd5\"}"));

    EXPECT_CALL(mockWindows, WriteToFile("dependencies/test1.json", "somethingnew", true))
        .Times(1);

    EXPECT_CALL(
            mockWindows,
            WriteToFile("dependencies/filehash.json", "{\n    \"test1.json\": \"nottestmd5\"\n}", true)
        )
        .Times(1);

    std::string expectedManifest1 =  "{\n    \"manifest\": {\n        \"test1.json\": {\n            \"md5\":";
    std::string expectedManifest2 =  " \"nottestmd5\",\n            \"uri\": \"http://test1.com\"\n        }\n    }\n}";
    std::string expectedManfiest = expectedManifest1 + expectedManifest2;
    EXPECT_CALL(
        mockWindows,
        WriteToFile(
            "dependencies/manifest.json",
            expectedManfiest,
            true
        ))
        .Times(1);

    EXPECT_CALL(mockWindows, ReadFromFileMock("dependencies/test1.json", true))
        .Times(1)
        .WillOnce(Return("testdata"));

    DependencyCache cache = DependencyBootstrap::Bootstrap(mockApi, mockWindows, mockCurl);
    EXPECT_EQ(1, cache.DependencyCount());
    EXPECT_TRUE(cache.HasDependency("test1.json"));
    EXPECT_TRUE(cache.GetDependency("test1.json") == "testdata");
}
}  // namespace Bootstrap
}  // namespace UKControllerPluginTest
