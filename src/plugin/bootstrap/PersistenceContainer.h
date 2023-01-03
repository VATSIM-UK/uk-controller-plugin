#pragma once

namespace UKControllerPlugin {
    class UKPlugin;

    namespace Aircraft {
        class AircraftTypeCollection;
        class AircraftTypeMapperInterface;
        class CallsignSelectionListFactory;
    } // namespace Aircraft
    namespace Airfield {
        class AirfieldCollection;
    } // namespace Airfield
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Bootstrap {
        class BootstrapProviderCollection;
        class ModuleFactories;
    } // namespace Bootstrap
    namespace Command {
        class CommandHandlerCollection;
    } // namespace Command
    namespace Controller {
        class ActiveCallsignCollection;
        class ControllerPositionCollection;
        class ControllerPositionHierarchyFactory;
        class ControllerStatusEventHandlerCollection;
        class HandoffEventHandlerCollection;
        class Login;
    } // namespace Controller
    namespace Curl {
        class CurlInterface;
    } // namespace Curl
    namespace Countdown {
        class CountdownTimer;
        class TimerConfigurationManager;
    } // namespace Countdown
    namespace Datablock {
        class DisplayTime;
    } // namespace Datablock
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class PluginSettingsProviderCollection;
        class RadarTargetEventHandlerCollection;
        class RunwayDialogAwareCollection;
        class UserSetting;
        class UserSettingAwareCollection;
    } // namespace Euroscope
    namespace Flightplan {
        class FlightPlanEventHandlerCollection;
        class StoredFlightplanCollection;
    } // namespace Flightplan
    namespace FlightRules {
        class FlightRuleCollection;
    } // namespace FlightRules
    namespace HistoryTrail {
        class HistoryTrailRepository;
    } // namespace HistoryTrail
    namespace Hold {
        class HoldManager;
        class HoldSelectionMenu;
        class HoldDisplayFactory;
        class PublishedHoldCollection;
    } // namespace Hold
    namespace Integration {
        class ExternalMessageEventHandler;
        struct IntegrationPersistenceContainer;
    } // namespace Integration
    namespace InitialAltitude {
        class InitialAltitudeEventHandler;
    } // namespace InitialAltitude
    namespace List {
        class PopupListFactory;
    } // namespace List
    namespace Metar {
        class MetarEventHandlerCollection;
    } // namespace Metar
    namespace Message {
        class UserMessager;
    } // namespace Message
    namespace MinStack {
        class MinStackManager;
    } // namespace MinStack
    namespace Navaids {
        class NavaidCollection;
    } // namespace Navaids
    namespace Notifications {
        class NotificationsMenuItem;
    } // namespace Notifications
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership
    namespace Plugin {
        class FunctionCallEventHandler;
    } // namespace Plugin
    namespace Prenote {
        class PrenoteMessageCollection;
        class PrenoteMessageEventHandlerCollection;
    } // namespace Prenote
    namespace Push {
        class PushEventProcessorCollection;
    } // namespace Push
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ScreenControls;
    } // namespace RadarScreen
    namespace Regional {
        class RegionalPressureManager;
    } // namespace Regional
    namespace Releases {
        class DepartureReleaseEventHandler;
    } // namespace Releases
    namespace Runway {
        class RunwayCollection;
    } // namespace Runway
    namespace SectorFile {
        class RunwayCollection;
    } // namespace SectorFile
    namespace Setting {
        class SettingRepository;
    } // namespace Setting
    namespace Sid {
        class SidCollection;
        class SidMapperInterface;
    } // namespace Sid
    namespace Squawk {
        class SquawkAssignment;
        class SquawkEventHandler;
        class SquawkGenerator;
    } // namespace Squawk
    namespace Tag {
        class TagItemCollection;
    } // namespace Tag
    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager
    namespace TimedEvent {
        class TimedEventCollection;
    } // namespace TimedEvent
    namespace Wake {
        class WakeCategoryMapperCollection;
        class WakeSchemeCollection;
    } // namespace Wake
    namespace Windows {
        struct GdiplusBrushes;
        class GdiGraphicsWrapper;
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPluginUtils {
    namespace Api {
        class ApiFactory;
    } // namespace Api
} // namespace UKControllerPluginUtils

namespace UKControllerPlugin::Bootstrap {

    /*
        A structure in which we persist pointers to things that need to be
        available throughout plugin execution.
    */
    using PersistenceContainer = struct PersistenceContainer
    {
        PersistenceContainer();
        ~PersistenceContainer();
        PersistenceContainer(const PersistenceContainer&) = delete;
        PersistenceContainer(PersistenceContainer&&) noexcept;
        auto operator=(const PersistenceContainer&) -> PersistenceContainer& = delete;
        auto operator=(PersistenceContainer&&) noexcept -> PersistenceContainer&;

        // The useful things
        std::unique_ptr<UKControllerPlugin::Curl::CurlInterface> curl;
        std::unique_ptr<UKControllerPlugin::Windows::WinApiInterface> windows;

        // The helpers and collections
        std::unique_ptr<UKControllerPlugin::Api::ApiInterface> api;
        std::shared_ptr<UKControllerPluginUtils::Api::ApiFactory> apiFactory;
        std::shared_ptr<UKControllerPlugin::TaskManager::TaskRunnerInterface> taskRunner;
        std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsignCollection> activeCallsigns;
        std::unique_ptr<UKControllerPlugin::Flightplan::StoredFlightplanCollection> flightplans;
        std::unique_ptr<UKControllerPlugin::Message::UserMessager> userMessager;
        std::unique_ptr<UKControllerPlugin::Euroscope::UserSetting> pluginUserSettingHandler;
        std::unique_ptr<UKControllerPlugin::Euroscope::PluginSettingsProviderCollection> pluginSettingsProviders;
        std::shared_ptr<UKControllerPlugin::Controller::Login> login;
        std::unique_ptr<UKControllerPlugin::Dialog::DialogManager> dialogManager;
        std::unique_ptr<UKControllerPlugin::Setting::SettingRepository> settingsRepository;
        std::shared_ptr<UKControllerPlugin::Datablock::DisplayTime> timeFormatting;
        std::shared_ptr<UKControllerPlugin::Prenote::PrenoteMessageCollection> prenotes;
        std::unique_ptr<UKControllerPlugin::Prenote::PrenoteMessageEventHandlerCollection> prenoteMessageHandlers;
        std::shared_ptr<UKControllerPlugin::Dependency::DependencyLoaderInterface> dependencyLoader;

        // Collections of event handlers
        std::unique_ptr<UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection> flightplanHandler;
        std::unique_ptr<UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection> controllerHandler;
        std::unique_ptr<UKControllerPlugin::Euroscope::RadarTargetEventHandlerCollection> radarTargetHandler;
        std::unique_ptr<UKControllerPlugin::TimedEvent::TimedEventCollection> timedHandler;
        std::unique_ptr<UKControllerPlugin::Tag::TagItemCollection> tagHandler;
        std::unique_ptr<UKControllerPlugin::Metar::MetarEventHandlerCollection> metarEventHandler;
        std::unique_ptr<UKControllerPlugin::RadarScreen::ScreenControls> screenControls;
        std::unique_ptr<UKControllerPlugin::Command::CommandHandlerCollection> commandHandlers;
        std::shared_ptr<Euroscope::UserSettingAwareCollection> userSettingHandlers;
        std::unique_ptr<UKControllerPlugin::Euroscope::RunwayDialogAwareCollection> runwayDialogEventHandlers;
        std::unique_ptr<UKControllerPlugin::Controller::HandoffEventHandlerCollection> controllerHandoffHandlers;
        std::shared_ptr<UKControllerPlugin::Integration::ExternalMessageEventHandler> externalEventHandler;
        std::unique_ptr<Integration::IntegrationPersistenceContainer> integrationModuleContainer;

        // The plugin
        std::unique_ptr<UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface> plugin;

        // The modules
        std::unique_ptr<ModuleFactories> moduleFactories;
        std::unique_ptr<BootstrapProviderCollection> bootstrapProviders;
        std::shared_ptr<UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler> initialAltitudeEvents;
        std::unique_ptr<UKControllerPlugin::HistoryTrail::HistoryTrailRepository> historyTrails;
        std::shared_ptr<UKControllerPlugin::Countdown::CountdownTimer> countdownTimer;
        std::shared_ptr<UKControllerPlugin::Countdown::TimerConfigurationManager> timerConfigurationManager;
        std::shared_ptr<UKControllerPlugin::MinStack::MinStackManager> minStack;
        std::shared_ptr<UKControllerPlugin::Regional::RegionalPressureManager> regionalPressureManager;
        std::unique_ptr<UKControllerPlugin::Squawk::SquawkAssignment> squawkAssignmentRules;
        std::shared_ptr<UKControllerPlugin::Squawk::SquawkEventHandler> squawkEvents;
        std::unique_ptr<UKControllerPlugin::Squawk::SquawkGenerator> squawkGenerator;
        std::unique_ptr<UKControllerPlugin::Hold::HoldManager> holdManager;
        std::shared_ptr<UKControllerPlugin::Hold::HoldSelectionMenu> holdSelectionMenu;
        std::unique_ptr<UKControllerPlugin::Hold::HoldDisplayFactory> holdDisplayFactory;
        std::shared_ptr<UKControllerPlugin::Notifications::NotificationsMenuItem> notificationsMenuItem;
        std::shared_ptr<UKControllerPlugin::Releases::DepartureReleaseEventHandler> departureReleaseHandler;

        // Collections that are spawned multiple times.
        std::vector<std::shared_ptr<UKControllerPlugin::RadarScreen::RadarRenderableCollection>> allRadarRenders;
        std::unique_ptr<UKControllerPlugin::Plugin::FunctionCallEventHandler> pluginFunctionHandlers;

        // Graphics things
        std::unique_ptr<UKControllerPlugin::Windows::GdiplusBrushes> brushes;
        std::unique_ptr<UKControllerPlugin::Windows::GdiGraphicsWrapper> graphics;

        // Large collections that we don't want to go onto the stack
        std::unique_ptr<Sid::SidCollection> sids;
        std::unique_ptr<Sid::SidMapperInterface> sidMapper;
        std::unique_ptr<const UKControllerPlugin::Aircraft::AircraftTypeCollection> aircraftTypes;
        std::unique_ptr<UKControllerPlugin::Aircraft::AircraftTypeMapperInterface> aircraftTypeMapper;
        std::unique_ptr<const UKControllerPlugin::Airfield::AirfieldCollection> airfields;
        std::shared_ptr<UKControllerPlugin::Ownership::AirfieldServiceProviderCollection> airfieldOwnership;
        std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionCollection> controllerPositions;
        std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchyFactory> controllerHierarchyFactory;
        std::shared_ptr<UKControllerPlugin::SectorFile::RunwayCollection> runways;
        std::shared_ptr<UKControllerPlugin::Navaids::NavaidCollection> navaids;
        std::unique_ptr<UKControllerPlugin::Wake::WakeSchemeCollection> wakeSchemes;
        std::unique_ptr<UKControllerPlugin::Wake::WakeCategoryMapperCollection> wakeCategoryMappers;
        std::shared_ptr<UKControllerPlugin::Hold::PublishedHoldCollection> publishedHolds;
        std::unique_ptr<UKControllerPlugin::FlightRules::FlightRuleCollection> flightRules;
        std::unique_ptr<UKControllerPlugin::Runway::RunwayCollection> runwayCollection;

        // Push events
        std::shared_ptr<Push::PushEventProcessorCollection> pushEventProcessors;

        // Some factories
        std::unique_ptr<Aircraft::CallsignSelectionListFactory> callsignSelectionListFactory;
        std::unique_ptr<List::PopupListFactory> popupListFactory;
    };
} // namespace UKControllerPlugin::Bootstrap
