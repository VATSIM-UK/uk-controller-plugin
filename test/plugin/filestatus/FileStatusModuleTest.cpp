#include "filestatus/FileStatusModule.h"
#include "bootstrap/PersistenceContainer.h"

using ::testing::Test;

namespace UKControllerPluginTest {
    namespace FileStatusModule {

        class FileStatusModuleTest : public Test
        {
            public:
            FileStatusModuleTest() = default;
        };

        TEST_F(FileStatusModuleTest, BasicSanityCheck)
        {
            UKControllerPlugin::Bootstrap::PersistenceContainer container;
            UKControllerPlugin::FileStatus::FileStatusModule module;

            module.BootstrapPlugin(container);

            EXPECT_TRUE(true);
        }

    } // namespace FileStatusModule
} // namespace UKControllerPluginTest