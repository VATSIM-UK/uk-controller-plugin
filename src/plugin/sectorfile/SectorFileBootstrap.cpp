#include "RunwayCollection.h"
#include "SectorFileBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "plugin/UKPlugin.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;

namespace UKControllerPlugin::SectorFile {

    void BootstrapPlugin(PersistenceContainer& container)
    {
        container.runways = std::make_shared<RunwayCollection>(static_cast<UKPlugin&>(*container.plugin));
        container.runwayDialogEventHandlers->AddHandler(container.runways);
    }

    void BootstrapRadarScreen(
        const PersistenceContainer& container, AsrEventHandlerCollection& asrEvents

    )
    {
        // Register the runway collection to be updated when the ASR loads
        asrEvents.RegisterHandler(container.runways);
    }
} // namespace UKControllerPlugin::SectorFile
