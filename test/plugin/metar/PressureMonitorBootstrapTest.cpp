#include "bootstrap/PersistenceContainer.h"
#include "metar/PressureMonitorBootstrap.h"
#include "metar/MetarEventHandlerCollection.h"
#include "euroscope/UserSettingAwareCollection.h"

using testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;

namespace UKControllerPluginTest {
    namespace Metar {

        class PressureMonitorBootstrapTest : public Test
        {
            public:
            PressureMonitorBootstrapTest()
            {
                container.metarEventHandler = std::make_unique<MetarEventHandlerCollection>();
                container.userSettingHandlers = std::make_unique<UserSettingAwareCollection>();
            }

            PersistenceContainer container;
        };

        TEST_F(PressureMonitorBootstrapTest, ItAddsPressureMonitorToMetarEvents)
        {
            UKControllerPlugin::Metar::PressureMonitorBootstrap(container);
            EXPECT_EQ(1, container.metarEventHandler->CountHandlers());
        }

        TEST_F(PressureMonitorBootstrapTest, ItAddsPressureMonitorToUserSettingHandlers)
        {
            UKControllerPlugin::Metar::PressureMonitorBootstrap(container);
            EXPECT_EQ(1, container.userSettingHandlers->Count());
        }
    } // namespace Metar
} // namespace UKControllerPluginTest
