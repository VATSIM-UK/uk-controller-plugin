#include "pch/pch.h"
#include "bootstrap/InitialisePlugin.h"
#include "api/ApiAuthChecker.h"
#include "bootstrap/CollectionBootstrap.h"
#include "duplicate/DuplicatePlugin.h"
#include "bootstrap/EventHandlerCollectionBootstrap.h"
#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/HelperBootstrap.h"
#include "bootstrap/PostInit.h"
#include "controller/ControllerBootstrap.h"
#include "countdown/CountdownModule.h"
#include "datablock/DatablockBoostrap.h"
#include "dependency/DependencyLoader.h"
#include "dependency/UpdateDependencies.h"
#include "euroscope/GeneralSettingsConfigurationBootstrap.h"
#include "euroscope/PluginUserSettingBootstrap.h"
#include "flightinformationservice/FlightInformationServiceModule.h"
#include "flightplan/FlightplanStorageBootstrap.h"
#include "handoff/HandoffModule.h"
#include "historytrail/HistoryTrailModule.h"
#include "hold/HoldModule.h"
#include "initialaltitude/InitialAltitudeModule.h"
#include "integration/IntegrationModule.h"
#include "intention/IntentionCodeModule.h"
#include "log/LoggerBootstrap.h"
#include "login/LoginModule.h"
#include "message/UserMessagerBootstrap.h"
#include "metar/PressureMonitorBootstrap.h"
#include "minstack/MinStackModule.h"
#include "navaids/NavaidModule.h"
#include "notifications/NotificationsModule.h"
#include "offblock/ActualOffBlockTimeBootstrap.h"
#include "offblock/EstimatedDepartureTimeBootstrap.h"
#include "offblock/EstimatedOffBlockTimeBootstrap.h"
#include "ownership/AirfieldOwnershipModule.h"
#include "plugin/UKPlugin.h"
#include "plugin/UkPluginBootstrap.h"
#include "prenote/PrenoteModule.h"
#include "regional/RegionalPressureModule.h"
#include "releases/ReleaseModule.h"
#include "sectorfile/SectorFileBootstrap.h"
#include "squawk/SquawkModule.h"
#include "srd/SrdModule.h"
#include "stands/StandModule.h"
#include "update/PluginVersion.h"
#include "wake/WakeModule.h"
#include "push/PushEventBootstrap.h"
#include "oceanic/OceanicModule.h"
#include "sid/SidModule.h"
#include "initialheading/InitialHeadingModule.h"

using UKControllerPlugin::Api::ApiAuthChecker;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Bootstrap::ExternalsBootstrap;
using UKControllerPlugin::Log::LoggerBootstrap;
using UKControllerPlugin::Bootstrap::HelperBootstrap;
using UKControllerPlugin::Bootstrap::CollectionBootstrap;
using UKControllerPlugin::Bootstrap::EventHandlerCollectionBootstrap;
using UKControllerPlugin::Bootstrap::UkPluginBootstrap;
using UKControllerPlugin::InitialAltitude::InitialAltitudeModule;
using UKControllerPlugin::IntentionCode::IntentionCodeModule;
using UKControllerPlugin::HistoryTrail::HistoryTrailModule;
using UKControllerPlugin::Controller::LoginModule;
using UKControllerPlugin::Ownership::AirfieldOwnershipModule;
using UKControllerPlugin::Countdown::CountdownModule;
using UKControllerPlugin::MinStack::MinStackModule;
using UKControllerPlugin::Regional::RegionalPressureModule;
using UKControllerPlugin::Bootstrap::PostInit;
using UKControllerPlugin::Plugin::PluginVersion;
using UKControllerPlugin::Squawk::SquawkModule;
using UKControllerPlugin::Flightplan::FlightplanStorageBootstrap;
using UKControllerPlugin::Datablock::ActualOffBlockTimeBootstrap;
using UKControllerPlugin::Datablock::EstimatedOffBlockTimeBootstrap;
using UKControllerPlugin::Prenote::PrenoteModule;
using UKControllerPlugin::Message::UserMessagerBootstrap;
using UKControllerPlugin::Euroscope::PluginUserSettingBootstrap;
using UKControllerPlugin::Duplicate::DuplicatePlugin;
using UKControllerPlugin::Datablock::EstimatedDepartureTimeBootstrap;
using UKControllerPlugin::Euroscope::GeneralSettingsConfigurationBootstrap;
using UKControllerPlugin::Dependency::DependencyLoader;

namespace UKControllerPlugin {
    /*
        Creates a dummy plugin to fill the spot.
    */
    void InitialisePlugin::CreateDummy(void)
    {
        this->fallbackPlugin = std::unique_ptr<EuroScopePlugIn::CPlugIn>(
            new EuroScopePlugIn::CPlugIn(
                EuroScopePlugIn::COMPATIBILITY_CODE,
                "UK Controller Plugin (Disabled)",
                "XXX",
                "VATSIM UK",
                "VATSIM United Kingdom Division"
            )
        );
    }

    /*
        Clean up after ourself as we get closed down.
    */
    void InitialisePlugin::EuroScopeCleanup(void)
    {
        // Shut down the container, task runner and logs.
        this->container->taskRunner.reset();
        this->container.reset();
        this->duplicatePlugin.reset();

        // Shut down winsock
        if (this->winsockInitialised) {
            LogInfo("Performing winsock cleanup");
            WSACleanup();
        }

        // Shut down GDI
        Gdiplus::GdiplusShutdown(this->gdiPlusToken);
        LogInfo("Plugin shutdown");
        ShutdownLogger();
    }

    /*
        Returns the plugin instance.
    */
    EuroScopePlugIn::CPlugIn * InitialisePlugin::GetPlugin(void)
    {
        return this->container->plugin.get();
    }

    /*
        After we've done the main DLL loading part, it's safe to do things like starting threads.
        So we initialise all the core plugin elements here and return the plugin instance.
    */
    void InitialisePlugin::PostInit(HINSTANCE dllInstance)
    {
        // Start GdiPlus
        Gdiplus::GdiplusStartupInput gdiStartup;
        GdiplusStartup(&this->gdiPlusToken, &gdiStartup, NULL);

        // Check if we're a duplicate plugin
        this->duplicatePlugin = std::make_unique<DuplicatePlugin>();
        this->container = std::make_unique<PersistenceContainer>();

        // Do helpers.
        EventHandlerCollectionBootstrap::BoostrapPlugin(*this->container);

        // Bootstrap the plugin itself
        UkPluginBootstrap::BootstrapPlugin(*this->container);
        PluginUserSettingBootstrap::BootstrapPlugin(*this->container);

        ExternalsBootstrap::Bootstrap(*this->container, dllInstance);
        ExternalsBootstrap::SetupUkcpFolderRoot(*this->container->windows);

        // Bootstrap the logger
        if (this->duplicatePlugin->Duplicate()) {
            LoggerBootstrap::Bootstrap(
                *this->container->windows,
                L"plugin-secondary"
            );
            LogInfo(
                std::string("Another instance of UKCP has been detected, automated actions such as squawk assignments ")
                + "will be performed by the primary plugin instance."
            );
        } else {
            LoggerBootstrap::Bootstrap(
                *this->container->windows,
                L"plugin"
            );
            LogInfo("Plugin loaded as primary instance");
        }

        // User messager
        UserMessagerBootstrap::BootstrapPlugin(*this->container);

        // API + Websocket
        HelperBootstrap::Bootstrap(*this->container);
        Push::BootstrapPlugin(*this->container, this->duplicatePlugin->Duplicate());

        // Datetime
        Datablock::BootstrapPlugin(*this->container);

        // If we're not allowed to use the API because we've been banned or something... It's no go.
        bool apiAuthorised = ApiAuthChecker::IsAuthorised(
            *this->container->api,
            *this->container->windows,
            *this->container->settingsRepository
        );

        // Dependency loading can happen regardless of plugin version or API status.
        Dependency::UpdateDependencies(
            *this->container->api,
            *this->container->windows
        );
        DependencyLoader loader(
            *this->container->windows
        );

        // Integration module and winsock
        WSADATA winsockData;
        int winsockStartupResult = WSAStartup(MAKEWORD(2, 2), &winsockData);
        if (winsockStartupResult != 0) {
            winsockInitialised = false;
            LogError("Error initialising winsock for integration server: " + std::to_string(winsockStartupResult));
        } else {
            LogInfo("Initialised winsock for integration server, version: "
                + std::to_string(winsockData.wVersion & 0xff) + "." + std::to_string(winsockData.wVersion >> 8 & 0xff)
            );
            winsockInitialised = true;
        }

        Integration::BootstrapPlugin(*this->container, duplicatePlugin->Duplicate(), winsockInitialised);

        // Boostrap all the modules at a plugin level
        Controller::BootstrapPlugin(*this->container, loader);
        CollectionBootstrap::BootstrapPlugin(*this->container, loader);
        FlightplanStorageBootstrap::BootstrapPlugin(*this->container);
        AirfieldOwnershipModule::BootstrapPlugin(*this->container, loader);
        Sid::BootstrapPlugin(*this->container, loader);
        Navaids::BootstrapPlugin(*this->container, loader);
        Releases::BootstrapPlugin(*this->container, *this->container->plugin, loader);
        Stands::BootstrapPlugin(*this->container, loader);
        Notifications::BootstrapPlugin(*this->container);
        FlightInformationService::BootstrapPlugin(*this->container);
        Oceanic::BootstrapPlugin(*this->container);

        Wake::BootstrapPlugin(*this->container, loader);
        LoginModule::BootstrapPlugin(*this->container);
        SectorFile::BootstrapPlugin(*this->container);

        // General settings config bootstrap
        GeneralSettingsConfigurationBootstrap::BootstrapPlugin(
            *this->container->dialogManager,
            *this->container->pluginUserSettingHandler,
            *this->container->userSettingHandlers
        );

        // Bootstrap the modules
        InitialAltitudeModule::BootstrapPlugin(*this->container);
        InitialHeading::BootstrapPlugin(*this->container);
        Srd::BootstrapPlugin(*this->container);
        IntentionCodeModule::BootstrapPlugin(*this->container);
        HistoryTrailModule::BootstrapPlugin(*this->container);
        CountdownModule::BootstrapPlugin(*this->container);
        MinStackModule::BootstrapPlugin(
            this->container->minStack,
            *this->container->taskRunner,
            *this->container->api,
            *this->container->pushEventProcessors,
            *this->container->dialogManager
        );
        RegionalPressureModule::BootstrapPlugin(
            this->container->regionalPressureManager,
            *this->container->taskRunner,
            *this->container->api,
            *this->container->pushEventProcessors,
            *this->container->dialogManager,
            loader
        );
        Hold::BootstrapPlugin(
            loader,
            *this->container,
            *this->container->userMessager
        );

        // Don't allow automatic squawk assignment if the plugin is deemed to be a duplicate
        SquawkModule::BootstrapPlugin(
            *this->container,
            this->duplicatePlugin->Duplicate()
        );

        PrenoteModule::BootstrapPlugin(*this->container, loader);
        Handoff::BootstrapPlugin(*this->container, loader);

        // Bootstrap other things
        ActualOffBlockTimeBootstrap::BootstrapPlugin(*this->container);
        EstimatedOffBlockTimeBootstrap::BootstrapPlugin(*this->container);
        EstimatedDepartureTimeBootstrap::BootstrapPlugin(*this->container);

        // Pressure monitor
        Metar::PressureMonitorBootstrap(*this->container);

        // Do post-init and final setup, which involves running tasks that need to happen on load.
        PostInit::Process(*this->container);
        LogInfo("Plugin loaded successfully");
    }
}  // namespace UKControllerPlugin
