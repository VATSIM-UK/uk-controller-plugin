#include "pch/pch.h"
#include "sectorfile/SectorFileBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "timedevent/TimedEventCollection.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Euroscope::RunwayDialogAwareCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace SectorFile {

        class SectorFileBootstrapTest : public Test
        {
            public:
                SectorFileBootstrapTest()
                {
                    container.runwayDialogEventHandlers.reset(new RunwayDialogAwareCollection);
                }

                AsrEventHandlerCollection asrEvents;
                PersistenceContainer container;
        };

        TEST_F(SectorFileBootstrapTest, ItRegistersForRunwayEvents)
        {
            UKControllerPlugin::SectorFile::BootstrapRadarScreen(this->container, this->asrEvents);
            EXPECT_EQ(1, this->container.runwayDialogEventHandlers->CountHandlers());
        }

        TEST_F(SectorFileBootstrapTest, ItRegistersForAsrEvents)
        {
            UKControllerPlugin::SectorFile::BootstrapRadarScreen(this->container, this->asrEvents);
            EXPECT_EQ(1, asrEvents.CountHandlers());
        }
    }  // namespace SectorFile
}  // namespace UKControllerPluginTest
