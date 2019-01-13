#include "pch/pch.h"
#include "api/RemoteFile.h"

using UKControllerPlugin::Api::RemoteFile;

namespace UKControllerPluginTest {
    namespace Api {

        TEST(RemoteFile, EqualityOperatorReturnsTrueIfFilenamesSame)
        {
            RemoteFile file1;
            file1.filename = "Test1";
            file1.hash = "TestMd5";
            file1.uri = "Uri";

            RemoteFile file2;
            file2.filename = "Test1";
            file2.hash = "NotTestMd5";
            file2.uri = "NotUri";

            EXPECT_TRUE(file1 == file2);
        }

        TEST(RemoteFile, EqualityOperatorReturnsTrueIfFilenamesDifferent)
        {
            RemoteFile file1;
            file1.filename = "Test1";
            file1.hash = "TestMd5";
            file1.uri = "Uri";

            RemoteFile file2;
            file2.filename = "Test2";
            file2.hash = "TestMd5";
            file2.uri = "Uri";

            EXPECT_FALSE(file1 == file2);
        }
    }  // namespace Api
}  // namespace UKControllerPluginTest
