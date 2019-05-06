#include "pch/stdafx.h"
#include "radarscreen/RadarScreenFactory.h"
#include "radarscreen/UKRadarScreen.h"
#include "bootstrap/PersistenceContainer.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "countdown/CountdownModule.h"
#include "minstack/MinStackModule.h"
#include "historytrail/HistoryTrailModule.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/ScreenControlsBootstrap.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/GeneralSettingsConfigurationBootstrap.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Countdown::CountdownModule;
using UKControllerPlugin::MinStack::MinStackModule;
using UKControllerPlugin::HistoryTrail::HistoryTrailModule;
using UKControllerPlugin::RadarScreen::ScreenControlsBootstrap;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Euroscope::GeneralSettingsConfigurationBootstrap;

namespace UKControllerPlugin {
    namespace RadarScreen {

        RadarScreenFactory::RadarScreenFactory(const PersistenceContainer & persistence)
            : persistence(persistence)
        {

        }

        /*
            Create the RadarScreen.
        */
        UKControllerPlugin::UKRadarScreen * RadarScreenFactory::Create(void) const
        {
            // Create the collections
            RadarRenderableCollection renderers;
            AsrEventHandlerCollection userSettingHandlers;
            ConfigurableDisplayCollection configurableDisplays;
            CommandHandlerCollection commandHandlers;


            // Run bootstrap
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                *persistence.pluginFunctionHandlers,
                configurableDisplays,
                commandHandlers,
                *persistence.windows
            );

            HistoryTrailModule::BootstrapRadarScreen(
                *persistence.pluginFunctionHandlers,
                *persistence.historyTrails,
                renderers,
                *persistence.windows,
                configurableDisplays,
                userSettingHandlers,
                commandHandlers,
                *persistence.plugin
            );

            MinStackModule::BootstrapRadarScreen(
                *persistence.pluginFunctionHandlers,
                *persistence.minStack,
                renderers,
                configurableDisplays,
                *persistence.brushes,
                userSettingHandlers
            );

            CountdownModule::BootstrapRadarScreen(
                *persistence.pluginFunctionHandlers,
                *persistence.countdownTimer,
                renderers,
                configurableDisplays,
                *persistence.brushes,
                userSettingHandlers
            );

            // Last thing we do is ScreenControls
            ScreenControlsBootstrap::BootstrapRadarScreen(configurableDisplays, renderers, *persistence.brushes);
            return new UKRadarScreen(userSettingHandlers, renderers, commandHandlers, *this->persistence.graphics);
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
