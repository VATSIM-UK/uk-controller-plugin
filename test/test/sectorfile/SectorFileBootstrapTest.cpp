#include "pch/pch.h"
#include "sectorfile/SectorFileBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "timedevent/TimedEventCollection.h"
#include "sectorfile/SectorFileAwareCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::SectorFile::SectorFileAwareCollection;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace SectorFile {

        class SectorFileBootstrapTest : public Test
        {
            public:
                SectorFileBootstrapTest()
                {
                    container.timedHandler.reset(new TimedEventCollection);
                    container.sectorFileEventHandlers.reset(new SectorFileAwareCollection);
                }

                PersistenceContainer container;
        };

        TEST_F(SectorFileBootstrapTest, ItRegistersCheckerForTimedEvents)
        {
            UKControllerPlugin::SectorFile::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(1));
        }

        TEST_F(SectorFileBootstrapTest, ItRegistersRunwayUpdaterForSectorFileEvents)
        {
            UKControllerPlugin::SectorFile::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.sectorFileEventHandlers->CountHandlers());
        }
    }  // namespace SectorFile
}  // namespace UKControllerPluginTest
