#include "pch/stdafx.h"
#include "bootstrap/InitialisePlugin.h"
#include "plugin/UKPlugin.h"
#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/HelperBootstrap.h"
#include "log/LoggerBootstrap.h"
#include "bootstrap/CollectionBootstrap.h"
#include "bootstrap/EventHandlerCollectionBootstrap.h"
#include "dependency/DependencyBootstrap.h"
#include "plugin/UkPluginBootstrap.h"
#include "dependency/DependencyCache.h"
#include "initialaltitude/InitialAltitudeModule.h"
#include "intention/IntentionCodeModule.h"
#include "historytrail/HistoryTrailModule.h"
#include "login/LoginModule.h"
#include "airfield/AirfieldOwnershipModule.h"
#include "radarscreen/RadarScreenFactory.h"
#include "update/PluginUpdateChecker.h"
#include "countdown/CountdownModule.h"
#include "minstack/MinStackModule.h"
#include "bootstrap/PostInit.h"
#include "update/PluginVersion.h"
#include "squawk/SquawkModule.h"
#include "flightplan/FlightplanStorageBootstrap.h"
#include "offblock/ActualOffBlockTimeBootstrap.h"
#include "offblock/EstimatedOffBlockTimeBootstrap.h"
#include "prenote/PrenoteModule.h"
#include "message/UserMessagerBootstrap.h"
#include "euroscope/PluginUserSettingBootstrap.h"
#include "api/ApiAuthChecker.h"
#include "bootstrap/DuplicatePlugin.h"
#include "timedevent/DeferredEventBootstrap.h"
#include "offblock/EstimatedDepartureTimeBootstrap.h"
#include "wake/WakeModule.h"
#include "hold/HoldModule.h"
#include "dependency/DependencyProviderInterface.h"
#include "dependency/DependencyProviderFactory.h"
#include "metar/PressureMonitorBootstrap.h"
#include "euroscope/GeneralSettingsConfigurationBootstrap.h"

using UKControllerPlugin::Api::ApiAuthChecker;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Bootstrap::ExternalsBootstrap;
using UKControllerPlugin::Log::LoggerBootstrap;
using UKControllerPlugin::Bootstrap::HelperBootstrap;
using UKControllerPlugin::Bootstrap::CollectionBootstrap;
using UKControllerPlugin::Bootstrap::EventHandlerCollectionBootstrap;
using UKControllerPlugin::Bootstrap::DependencyBootstrap;
using UKControllerPlugin::Bootstrap::UkPluginBootstrap;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::InitialAltitude::InitialAltitudeModule;
using UKControllerPlugin::IntentionCode::IntentionCodeModule;
using UKControllerPlugin::HistoryTrail::HistoryTrailModule;
using UKControllerPlugin::Controller::LoginModule;
using UKControllerPlugin::Airfield::AirfieldOwnershipModule;
using UKControllerPlugin::Update::PluginUpdateChecker;
using UKControllerPlugin::Countdown::CountdownModule;
using UKControllerPlugin::MinStack::MinStackModule;
using UKControllerPlugin::Bootstrap::PostInit;
using UKControllerPlugin::Plugin::PluginVersion;
using UKControllerPlugin::Squawk::SquawkModule;
using UKControllerPlugin::Flightplan::FlightplanStorageBootstrap;
using UKControllerPlugin::Datablock::ActualOffBlockTimeBootstrap;
using UKControllerPlugin::Datablock::EstimatedOffBlockTimeBootstrap;
using UKControllerPlugin::Prenote::PrenoteModule;
using UKControllerPlugin::Message::UserMessagerBootstrap;
using UKControllerPlugin::Euroscope::PluginUserSettingBootstrap;
using UKControllerPlugin::Bootstrap::DuplicatePlugin;
using UKControllerPlugin::TimedEvent::DeferredEventBootstrap;
using UKControllerPlugin::Datablock::EstimatedDepartureTimeBootstrap;
using UKControllerPlugin::Dependency::DependencyProviderInterface;
using UKControllerPlugin::Dependency::GetDependencyProvider;
using UKControllerPlugin::Euroscope::GeneralSettingsConfigurationBootstrap;

namespace UKControllerPlugin {

    void InitialisePlugin::CheckForUpdates(const PersistenceContainer & container)
    {
        // If we're running a testing build of the plugin, skip the check for updates.
        #if defined (DISABLE_VERSION_CHECK)
                this->updateStatus = PluginUpdateChecker::versionAllowed;
        #else
                this->updateStatus = PluginUpdateChecker::CheckForUpdates(
                    PluginVersion::version,
                    *container.windows,
                    *container.api
                );
        #endif  // DISABLE_VERSION_CHECK
    }

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
        // Shut down the container.;
        this->container->taskRunner.reset();
        this->container.reset();

        // Shut down GDI
        Gdiplus::GdiplusShutdown(this->gdiPlusToken);
        LogInfo("Plugin shutdown");
    }

    /*
        Returns the plugin instance.
    */
    EuroScopePlugIn::CPlugIn * InitialisePlugin::GetPlugin(void)
    {
        if (this->updateStatus == PluginUpdateChecker::unsupportedVersion) {
            return this->fallbackPlugin.get();
        }

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
        Gdiplus::GdiplusStartup(&this->gdiPlusToken, &gdiStartup, NULL);

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
        LoggerBootstrap::Bootstrap(*this->container, this->duplicatePlugin->Duplicate());

        // API
        HelperBootstrap::Bootstrap(*this->container);

        // If we're not allowed to use the API because we've been banned or something... It's no go.
        bool apiAuthorised = ApiAuthChecker::IsAuthorised(
            *this->container->api,
            *this->container->windows,
            *this->container->settingsRepository
        );

        // Check for updates, but only if the API is either authorised
        this->updateStatus = PluginUpdateChecker::statusUnknown;
        if (apiAuthorised) {
            this->CheckForUpdates(*this->container);

            // If the plugin isn't an allowed version, stop here and put a dummy in place.
            if (this->updateStatus == PluginUpdateChecker::unsupportedVersion) {
                LogCritical("Deprecated plugin version, please download the latest update");
                this->CreateDummy();
                return;
            }
        }

        // Dependency loading can happen regardless of plugin version or API status.
        DependencyCache dependencyCache = DependencyBootstrap::Bootstrap(
            *this->container->api,
            *this->container->windows,
            *this->container->curl
        );

        // Load all the "new" dependencies that don't come from a manifest.
        std::unique_ptr<DependencyProviderInterface> dependencyProvider = GetDependencyProvider(
            *container->api,
            *container->windows,
            apiAuthorised
        );
        LogInfo("Loading new dependencies with provider " + dependencyProvider->GetProviderType());

        // Boostrap all the modules at a plugin level
        CollectionBootstrap::BootstrapPlugin(*this->container, dependencyCache);
        FlightplanStorageBootstrap::BootstrapPlugin(*this->container);

        // Bootstrap helpers
        UKControllerPlugin::Wake::BootstrapPlugin(*this->container, dependencyCache);
        LoginModule::BootstrapPlugin(*this->container);
        UserMessagerBootstrap::BootstrapPlugin(*this->container);
        DeferredEventBootstrap(*this->container->timedHandler);

        // General settings config bootstrap
        GeneralSettingsConfigurationBootstrap::BootstrapPlugin(
            *this->container->dialogManager,
            *this->container->pluginUserSettingHandler,
            *this->container->userSettingHandlers
        );

        // Bootstrap the modules

        // Only load initial altitudes if we know the plugin version is ok (as this modifies flightplans)
        // Don't load it if the plugin is a duplicate, leave that to the main one.
        if (
            this->updateStatus == PluginUpdateChecker::versionAllowed &&
            !this->duplicatePlugin->Duplicate()
        ) {
            InitialAltitudeModule::BootstrapPlugin(dependencyCache, *this->container);
        }

        UKControllerPlugin::Hold::BootstrapPlugin(
            *dependencyProvider,
            *this->container,
            *this->container->userMessager
        );
        IntentionCodeModule::BootstrapPlugin(*this->container);
        HistoryTrailModule::BootstrapPlugin(*this->container);
        CountdownModule::BootstrapPlugin(this->container->countdownTimer, *this->container->windows);
        MinStackModule::BootstrapPlugin(
            this->container->minStack,
            *this->container->metarEventHandler,
            *this->container->taskRunner,
            *this->container->curl
        );

        // Due to flightplan modifications and API interactions, only enable the squawk module
        // if the API is authorised and the plugin is an allowed version. Also, dont allow automatic
        // squawk assignment if the plugin is deemed to be a duplicate
        SquawkModule::BootstrapPlugin(
            *this->container,
            this->updateStatus != PluginUpdateChecker::versionAllowed,
            this->duplicatePlugin->Duplicate()
        );

        AirfieldOwnershipModule::BootstrapPlugin(*this->container, dependencyCache);
        PrenoteModule::BootstrapPlugin(*this->container, dependencyCache);

        // Bootstrap other things
        ActualOffBlockTimeBootstrap::BootstrapPlugin(*this->container);
        EstimatedOffBlockTimeBootstrap::BootstrapPlugin(*this->container);
        EstimatedDepartureTimeBootstrap::BootstrapPlugin(*this->container);

        // Pressure monitor
        UKControllerPlugin::Metar::PressureMonitorBootstrap(*this->container);

        // Do post-init and final setup, which involves running tasks that need to happen on load.
        PostInit::Process(*this->container);
        LogInfo("Plugin loaded successfully");
    }
}  // namespace UKControllerPlugin
