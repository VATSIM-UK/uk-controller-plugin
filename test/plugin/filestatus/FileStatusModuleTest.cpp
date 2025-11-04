#include "filestatus/FileStatusModule.h"

using ::testing::Test;

namespace UKControllerPluginTest {
    namespace FileStatusModule {

        class FileStatusModuleTest : public _test
        {
            public:
            FileStatusModuleTest()
            {
            }
            TEST_F(FileStatusModuleTest)
            {
                EXPECT_TRUE(1);
            }
        }

    } // namespace FileStatusModule

} // namespace UKControllerPluginTest