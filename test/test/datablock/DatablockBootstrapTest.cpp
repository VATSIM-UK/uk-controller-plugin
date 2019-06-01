#include "pch/pch.h"
#include "datablock/DatablockBoostrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/UserSettingAwareCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using testing::Test;

namespace UKControllerPluginTest {
    namespace Datablock {

        class DatablockBootstrapTest : public Test
        {
            public:

                DatablockBootstrapTest()
                {
                    this->container.userSettingHandlers = std::make_unique<UserSettingAwareCollection>();
                }

                PersistenceContainer container;
        };

        TEST_F(DatablockBootstrapTest, ItCreatesTimeFormatterOnContainer)
        {
            UKControllerPlugin::Datablock::BootstrapPlugin(this->container);
            EXPECT_NO_THROW(this->container.timeFormatting->GetUnknownTimeFormat());
        }

        TEST_F(DatablockBootstrapTest, ItAddsToUserSettingAware)
        {
            UKControllerPlugin::Datablock::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.userSettingHandlers->Count());
        }
    }  // namespace Datablock
}  // namespace UKControllerPluginTest
