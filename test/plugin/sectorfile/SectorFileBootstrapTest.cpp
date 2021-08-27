#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "sectorfile/RunwayCollection.h"
#include "sectorfile/SectorFileBootstrap.h"
#include "timedevent/TimedEventCollection.h"

using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::RunwayDialogAwareCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest::SectorFile {

    class SectorFileBootstrapTest : public Test
    {
        public:
        SectorFileBootstrapTest()
        {
            container.runwayDialogEventHandlers = std::make_unique<RunwayDialogAwareCollection>();
        }

        AsrEventHandlerCollection asrEvents;
        PersistenceContainer container;
    };

    TEST_F(SectorFileBootstrapTest, BootstrapPluginSetsUpTheCollection)
    {
        UKControllerPlugin::SectorFile::BootstrapPlugin(this->container);
        EXPECT_EQ(0, this->container.runways->Count());
    }

    TEST_F(SectorFileBootstrapTest, BootstrapPluginRegistersForRunwayDialogEvents)
    {
        UKControllerPlugin::SectorFile::BootstrapPlugin(this->container);
        EXPECT_EQ(1, this->container.runwayDialogEventHandlers->CountHandlers());
    }

    TEST_F(SectorFileBootstrapTest, ItRegistersForAsrEvents)
    {
        UKControllerPlugin::SectorFile::BootstrapRadarScreen(this->container, this->asrEvents);
        EXPECT_EQ(1, asrEvents.CountHandlers());
    }
} // namespace UKControllerPluginTest::SectorFile
