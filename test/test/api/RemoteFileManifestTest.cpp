#include "pch/pch.h"
#include "api/RemoteFileManifest.h"
#include "api/RemoteFile.h"

using UKControllerPlugin::Api::RemoteFileManifest;
using UKControllerPlugin::Api::RemoteFile;

namespace UKControllerPluginTest {
    namespace Api {

        TEST(RemoteFileManifest, AddFileThrowsExceptionIfFileAlreadyExists)
        {
            RemoteFile file;
            file.filename = "TestFile";
            file.hash = "md5";
            file.uri = "uri";

            RemoteFileManifest manifest;
            EXPECT_NO_THROW(manifest.AddFile(file));
            EXPECT_THROW(manifest.AddFile(file), std::invalid_argument);
        }

        TEST(RemoteFileManifest, TheManifestStartsEmpty)
        {
            RemoteFileManifest manifest;
            EXPECT_TRUE(manifest.IsEmpty());
        }

        TEST(RemoteFileManifest, TheManifestIsNotEmptyAfterFilesAdded)
        {
            RemoteFile file1;
            file1.filename = "TestFile1";
            file1.hash = "md5";
            file1.uri = "uri";

            RemoteFileManifest manifest;
            manifest.AddFile(file1);

            EXPECT_FALSE(manifest.IsEmpty());
        }

        TEST(RemoteFileManifest, ManifestIsIterable)
        {
            RemoteFile file1;
            file1.filename = "TestFile1";
            file1.hash = "md5";
            file1.uri = "uri";

            RemoteFile file2;
            file2.filename = "TestFile2";
            file2.hash = "md5";
            file2.uri = "uri";

            RemoteFile file3;
            file3.filename = "TestFile3";
            file3.hash = "md5";
            file3.uri = "uri";

            RemoteFileManifest manifest;
            manifest.AddFile(file1);
            manifest.AddFile(file2);
            manifest.AddFile(file3);

            RemoteFileManifest::iterator it = manifest.begin();
            EXPECT_EQ(file1, *it++);
            EXPECT_EQ(file2, *it++);
            EXPECT_EQ(file3, *it++);
        }

        TEST(RemoteFileManifest, ManifestIsIterableAsConstant)
        {
            RemoteFile file1;
            file1.filename = "TestFile1";
            file1.hash = "md5";
            file1.uri = "uri";

            RemoteFile file2;
            file2.filename = "TestFile2";
            file2.hash = "md5";
            file2.uri = "uri";

            RemoteFile file3;
            file3.filename = "TestFile3";
            file3.hash = "md5";
            file3.uri = "uri";

            RemoteFileManifest manifest;
            manifest.AddFile(file1);
            manifest.AddFile(file2);
            manifest.AddFile(file3);

            RemoteFileManifest::const_iterator it = manifest.cbegin();
            EXPECT_EQ(file1, *it++);
            EXPECT_EQ(file2, *it++);
            EXPECT_EQ(file3, *it++);
        }

        TEST(RemoteFileManifest, ManifestCanBeSerializedIntoJson)
        {
            RemoteFile file1;
            file1.filename = "TestFile1";
            file1.hash = "md51";
            file1.uri = "uri1";

            RemoteFile file2;
            file2.filename = "TestFile2";
            file2.hash = "md52";
            file2.uri = "uri2";

            RemoteFile file3;
            file3.filename = "TestFile3";
            file3.hash = "md53";
            file3.uri = "uri3";

            RemoteFileManifest manifest;
            manifest.AddFile(file1);
            manifest.AddFile(file2);
            manifest.AddFile(file3);

            nlohmann::json expectedJson;
            expectedJson["manifest"] = {};
            expectedJson["manifest"]["TestFile1"] = { { "uri", "uri1" }, { "md5", "md51" } };
            expectedJson["manifest"]["TestFile2"] = { { "uri", "uri2" }, { "md5", "md52" } };
            expectedJson["manifest"]["TestFile3"] = { { "uri", "uri3" }, { "md5", "md53" } };

            EXPECT_TRUE(expectedJson.dump(4) == manifest.ToJsonString());
        }
    }  // namespace Api
}  // namespace UKControllerPluginTest
