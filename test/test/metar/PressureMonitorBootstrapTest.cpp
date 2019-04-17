#include "pch/pch.h"
#include "bootstrap/PersistenceContainer.h"
#include "metar/PressureMonitorBootstrap.h"
#include "metar/MetarEventHandlerCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using testing::Test;

namespace UKControllerPluginTest {
    namespace Metar {

        class PressureMonitorBootstrapTest : public Test
        {
            public:
                PressureMonitorBootstrapTest()
                {
                    container.metarEventHandler = std::make_unique<MetarEventHandlerCollection>();
                }

                PersistenceContainer container;
        };

        TEST_F(PressureMonitorBootstrapTest, ItAddsPressureMonitorToMetarEvents)
        {
            UKControllerPlugin::Metar::PressureMonitorBootstrap(container);
            EXPECT_EQ(1, container.metarEventHandler->CountHandlers());
        }
    }  // namespace Metar
}  // namespace UKControllerPluginTest
