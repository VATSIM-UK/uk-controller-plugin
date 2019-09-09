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
#include "radarscreen/PositionResetCommand.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/GeneralSettingsConfigurationBootstrap.h"
#include "hold/HoldModule.h"
#include "bootstrap/HelperBootstrap.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Countdown::CountdownModule;
using UKControllerPlugin::MinStack::MinStackModule;
using UKControllerPlugin::HistoryTrail::HistoryTrailModule;
using UKControllerPlugin::RadarScreen::ScreenControlsBootstrap;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Euroscope::GeneralSettingsConfigurationBootstrap;
using UKControllerPlugin::Bootstrap::HelperBootstrap;

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
            this->renderableCollections.push_back(std::make_shared<RadarRenderableCollection>());
            RadarRenderableCollection & renderers = *renderableCollections.back();
            AsrEventHandlerCollection userSettingHandlers;
            ConfigurableDisplayCollection configurableDisplays;
            CommandHandlerCollection commandHandlers;

            // Run bootstrap
            HelperBootstrap::BootstrapApiConfigurationItem(
                persistence,
                configurableDisplays
            );

            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                *persistence.pluginFunctionHandlers,
                configurableDisplays,
                commandHandlers,
                *persistence.dialogManager
            );

            HistoryTrailModule::BootstrapRadarScreen(
                *persistence.pluginFunctionHandlers,
                *persistence.historyTrails,
                renderers,
                *persistence.dialogManager,
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
                userSettingHandlers,
                *persistence.dialogManager
            );

            CountdownModule::BootstrapRadarScreen(
                *persistence.pluginFunctionHandlers,
                *persistence.countdownTimer,
                persistence.timerConfigurationManager,
                renderers,
                configurableDisplays,
                *persistence.brushes,
                userSettingHandlers
            );

            UKControllerPlugin::Hold::BootstrapRadarScreen(
                configurableDisplays,
                renderers,
                userSettingHandlers,
                commandHandlers,
                this->persistence
            );

            // Register command for position resets
            this->persistence.commandHandlers->RegisterHandler(
                std::make_shared<PositionResetCommand>(renderers)
            );

            // Last thing we do is ScreenControls
            ScreenControlsBootstrap::BootstrapRadarScreen(configurableDisplays, renderers, *persistence.brushes);
            return new UKRadarScreen(userSettingHandlers, renderers, commandHandlers, *this->persistence.graphics);
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
