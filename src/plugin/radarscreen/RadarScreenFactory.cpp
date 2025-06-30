#include "ConfigurableDisplayCollection.h"
#include "MenuToggleableDisplayFactory.h"
#include "PositionResetCommand.h"
#include "RadarScreenFactory.h"
#include "ScreenControlsBootstrap.h"
#include "UKRadarScreen.h"
#include "api/BootstrapApi.h"
#include "approach/ApproachBootstrapProvider.h"
#include "bootstrap/BootstrapProviderCollection.h"
#include "bootstrap/HelperBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "countdown/CountdownModule.h"
#include "departure/DepartureModule.h"
#include "euroscope/GeneralSettingsConfigurationBootstrap.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "historytrail/HistoryTrailModule.h"
#include "hold/HoldModule.h"
#include "minstack/MinStackModule.h"
#include "missedapproach/MissedApproachModule.h"
#include "notifications/NotificationsModule.h"
#include "plugin/PluginInformationBootstrap.h"
#include "plugin/UKPlugin.h"
#include "prenote/PrenoteModule.h"
#include "regional/RegionalPressureModule.h"
#include "releases/ReleaseModule.h"
#include "sectorfile/SectorFileBootstrap.h"
#include "srd/SrdModule.h"
#include "wake/WakeModule.h"

using UKControllerPlugin::Api::BootstrapConfigurationMenuItem;
using UKControllerPlugin::Bootstrap::HelperBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Countdown::CountdownModule;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::GeneralSettingsConfigurationBootstrap;
using UKControllerPlugin::HistoryTrail::HistoryTrailModule;
using UKControllerPlugin::MinStack::MinStackModule;
using UKControllerPlugin::Prenote::PrenoteModule;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::RadarScreen::ScreenControlsBootstrap;
using UKControllerPlugin::Regional::RegionalPressureModule;

namespace UKControllerPlugin::RadarScreen {

    RadarScreenFactory::RadarScreenFactory(const PersistenceContainer& persistence) : persistence(persistence)
    {
    }

    /*
        Create the RadarScreen.
    */
    auto RadarScreenFactory::Create() const -> UKRadarScreen*
    {
        // Create the collections
        this->renderableCollections.push_back(std::make_shared<RadarRenderableCollection>());
        RadarRenderableCollection& renderers = *renderableCollections.back();
        AsrEventHandlerCollection userSettingHandlers;
        ConfigurableDisplayCollection configurableDisplays;
        CommandHandlerCollection commandHandlers;
        MenuToggleableDisplayFactory displayFactory(*this->persistence.pluginFunctionHandlers, configurableDisplays);

        // Run bootstrap
        BootstrapConfigurationMenuItem(persistence, configurableDisplays);

        SectorFile::BootstrapRadarScreen(persistence, userSettingHandlers);

        GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
            *persistence.pluginFunctionHandlers, configurableDisplays, *persistence.brushes, commandHandlers, *persistence.dialogManager);

        HistoryTrailModule::BootstrapRadarScreen(
            *persistence.pluginFunctionHandlers,
            *persistence.historyTrails,
            renderers,
            *persistence.dialogManager,
            configurableDisplays,
            userSettingHandlers,
            commandHandlers,
            *persistence.plugin);

        MinStackModule::BootstrapRadarScreen(
            *persistence.pluginFunctionHandlers,
            *persistence.minStack,
            renderers,
            configurableDisplays,
            *persistence.brushes,
            userSettingHandlers,
            *persistence.dialogManager);

        RegionalPressureModule ::BootstrapRadarScreen(
            *persistence.pluginFunctionHandlers,
            *persistence.regionalPressureManager,
            renderers,
            configurableDisplays,
            *persistence.brushes,
            userSettingHandlers,
            *persistence.dialogManager);

        CountdownModule::BootstrapRadarScreen(
            *persistence.pluginFunctionHandlers,
            *persistence.countdownTimer,
            persistence.timerConfigurationManager,
            renderers,
            configurableDisplays,
            *persistence.brushes,
            userSettingHandlers);

        Hold::BootstrapRadarScreen(
            configurableDisplays, renderers, userSettingHandlers, commandHandlers, *persistence.brushes, this->persistence);

        Srd::BootstrapRadarScreen(configurableDisplays);
        Notifications::BootstrapRadarScreen(this->persistence, configurableDisplays);
        Releases::BootstrapRadarScreen(this->persistence, renderers);
        PrenoteModule::BootstrapRadarScreen(this->persistence, renderers);
        Departure::BootstrapRadarScreen(this->persistence, renderers, configurableDisplays, userSettingHandlers);
        MissedApproach::BootstrapRadarScreen(this->persistence, renderers, configurableDisplays, userSettingHandlers);
        Wake::BootstrapRadarScreen(this->persistence, renderers, userSettingHandlers, displayFactory, *persistence.brushes);

        this->persistence.bootstrapProviders->BootstrapRadarScreen(
            this->persistence, renderers, configurableDisplays, userSettingHandlers, displayFactory);

        // Register command for position resets
        this->persistence.commandHandlers->RegisterHandler(std::make_shared<PositionResetCommand>(renderers));

        // Register the plugin information message box
        UKControllerPlugin::Plugin::BootstrapPluginInformationMessage(this->persistence, configurableDisplays);

        // Last thing we do is ScreenControls
        ScreenControlsBootstrap::BootstrapRadarScreen(configurableDisplays, renderers, *persistence.brushes);
        return new UKRadarScreen(
            userSettingHandlers,
            renderers,
            commandHandlers,
            *this->persistence.graphics,
            *persistence.pluginSettingsProviders,
            *persistence.pluginFunctionHandlers);
    }
} // namespace UKControllerPlugin::RadarScreen
