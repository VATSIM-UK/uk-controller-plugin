#include "pch/pch.h"
#include "api/RemoteFileManifestFactory.h"
#include "api/RemoteFileManifest.h"
#include "api/RemoteFile.h"
#include "mock/MockWinApi.h"

using UKControllerPlugin::Api::RemoteFileManifest;
using UKControllerPlugin::Api::RemoteFileManifestFactory;
using UKControllerPlugin::Api::RemoteFile;
using UKControllerPluginTest::Windows::MockWinApi;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Api {

        class RemoteFileManifestFactoryTest : public Test
        {
            public:
                RemoteFileManifestFactoryTest()
                    : manifestFactory(mockWindowsApi)
                {

                }

                RemoteFileManifestFactory manifestFactory;
                NiceMock<MockWinApi> mockWindowsApi;
        };

        TEST_F(RemoteFileManifestFactoryTest, CreateFromDataIgnoresMalformedDependencies)
        {
            RemoteFileManifest manifest = this->manifestFactory.CreateFromData(
               "{\"manifest\" : {\"test1.json\": {\"notmd5\": \"md5\", \"uri\": \"uri\"}}}"_json
            );

            EXPECT_TRUE(manifest.IsEmpty());
        }

        TEST_F(RemoteFileManifestFactoryTest, CreateFromDataCanHandleEmptyDependencies)
        {
            RemoteFileManifest manifest = this->manifestFactory.CreateFromData(
                "{}"_json
            );

            EXPECT_TRUE(manifest.IsEmpty());
        }

        TEST_F(RemoteFileManifestFactoryTest, CreateFromDataStopsIfManifestIsNotObject)
        {
            RemoteFileManifest manifest = this->manifestFactory.CreateFromData(
                "{\"manifest\" : \"lalal\"}"_json
            );

            EXPECT_TRUE(manifest.IsEmpty());
        }

        TEST_F(RemoteFileManifestFactoryTest, CreateFromDataBuildsAManifest)
        {
            RemoteFileManifest manifest = this->manifestFactory.CreateFromData(
                "{\"manifest\" : {\"test1.json\": {\"md5\": \"md5\", \"uri\": \"uri\"}}}"_json
            );

            RemoteFile expectedFile;
            expectedFile.filename = "test1.json";
            expectedFile.hash = "md5";
            expectedFile.uri = "uri";

            RemoteFileManifest::iterator it = manifest.begin();
            EXPECT_TRUE(expectedFile == *it);
            EXPECT_TRUE(++it == manifest.end());
        }

        TEST_F(RemoteFileManifestFactoryTest, CreateCanHandleMultipleItems)
        {
            std::string jsonPart1 = "{\"manifest\" :{\"test1.json\": {\"md5\": \"md5\", \"uri\": \"uri\"},";
            std::string jsonPart2 = "\"test2.json\": {\"md5\": \"md52\", \"uri\": \"uri2\"}}}";
            RemoteFileManifest manifest = this->manifestFactory.CreateFromData(
                nlohmann::json::parse(jsonPart1 + jsonPart2)
            );

            RemoteFile expectedFile1;
            expectedFile1.filename = "test1.json";
            expectedFile1.hash = "md5";
            expectedFile1.uri = "uri";

            RemoteFile expectedFile2;
            expectedFile2.filename = "test2.json";
            expectedFile2.hash = "md52";
            expectedFile2.uri = "uri2";

            RemoteFileManifest::iterator it = manifest.begin();
            EXPECT_TRUE(expectedFile1 == *it);
            EXPECT_TRUE(expectedFile2 == *++it);
            EXPECT_TRUE(++it == manifest.end());
        }

        TEST_F(RemoteFileManifestFactoryTest, CreatesFromLocalFileStopsIfNoLocalFile)
        {
            ON_CALL(this->mockWindowsApi, FileExists("dependencies/manifest.json"))
                .WillByDefault(Return(false));

            RemoteFileManifest manifest = this->manifestFactory.CreateFromLocalFile("dependencies/manifest.json");

            EXPECT_TRUE(manifest.IsEmpty());
        }

        TEST_F(RemoteFileManifestFactoryTest, CreatesFromLocalFileStopsIfManifestNotObject)
        {
            ON_CALL(this->mockWindowsApi, FileExists("dependencies/manifest.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindowsApi, ReadFromFileMock("dependencies/manifest.json", true))
                .WillByDefault(Return("{\"test1.json\": {\"md5\": \"md5\", \"uri\": \"uri\"}}"));

            RemoteFileManifest manifest = this->manifestFactory.CreateFromLocalFile("dependencies/manifest.json");

            EXPECT_TRUE(manifest.IsEmpty());
        }

        TEST_F(RemoteFileManifestFactoryTest, CreatesFromLocalFileReturnsAManifest)
        {
            ON_CALL(this->mockWindowsApi, FileExists("dependencies/manifest.json"))
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockWindowsApi, ReadFromFileMock("dependencies/manifest.json", true))
                .Times(1)
                .WillOnce(Return("{\"manifest\": {\"test1.json\": {\"md5\": \"md5\", \"uri\": \"uri\"}}}"));

            RemoteFileManifest manifest = this->manifestFactory.CreateFromLocalFile("dependencies/manifest.json");

            RemoteFile expectedFile;
            expectedFile.filename = "test1.json";
            expectedFile.hash = "md5";
            expectedFile.uri = "uri";

            RemoteFileManifest::iterator it = manifest.begin();
            EXPECT_TRUE(expectedFile == *it);
            EXPECT_TRUE(++it == manifest.end());
        }

        TEST_F(RemoteFileManifestFactoryTest, CreatesFromLocalFileStopsIfLocalFileCorrupt)
        {
            ON_CALL(this->mockWindowsApi, FileExists("dependencies/manifest.json"))
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockWindowsApi, ReadFromFileMock("dependencies/manifest.json", true))
                .Times(1)
                .WillOnce(Return("{test1.json.is.a.syntax.error: {\"md5\": \"md5\", \"uri\": \"uri\"}}"));

            RemoteFileManifest manifest = this->manifestFactory.CreateFromLocalFile("dependencies/manifest.json");

            EXPECT_TRUE(manifest.IsEmpty());
        }
    }  // namespace Api
}  // namespace UKControllerPluginTest
