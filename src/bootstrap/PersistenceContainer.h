#pragma once
#include "curl/CurlInterface.h"
#include "windows/WinApiInterface.h"
#include "api/ApiInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "airfield/AirfieldCollection.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "tag/TagItemCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "metar/MetarEventHandlerCollection.h"
#include "plugin/UKPlugin.h"
#include "radarscreen/RadarScreenFactory.h"
#include "initialaltitude/InitialAltitudeEventHandler.h"
#include "historytrail/HistoryTrailRepository.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ScreenControls.h"
#include "graphics/GdiplusBrushes.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "countdown/CountdownTimer.h"
#include "minstack/MinStackManager.h"
#include "squawk/SquawkAssignment.h"
#include "squawk/SquawkEventHandler.h"
#include "squawk/SquawkGenerator.h"
#include "controller/ControllerPositionCollection.h"
#include "intention/SectorExitRepository.h"
#include "message/UserMessager.h"
#include "euroscope/UserSetting.h"
#include "command/CommandHandlerCollection.h"
#include "login/Login.h"
#include "hold/HoldManager.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "hold/HoldSelectionMenu.h"
#include "dialog/DialogManager.h"
#include "hold/HoldDisplayFactory.h"
#include "setting/SettingRepository.h"
#include "countdown/TimerConfigurationManager.h"
#include "datablock/DisplayTime.h"
#include "push/PushEventProcessorCollection.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "sectorfile/RunwayCollection.h"
#include "handoff/HandoffCollection.h"
#include "regional/RegionalPressureManager.h"
#include "navaids/NavaidCollection.h"
#include "hold/PublishedHoldCollection.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "integration/ExternalMessageEventHandler.h"
#include "sid/SidCollection.h"
#include "integration/IntegrationPersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            A structure in which we persist pointers to things that need to be
            available throughout plugin execution.
        */
        typedef struct PersistenceContainer {

            // The useful things
            std::unique_ptr<UKControllerPlugin::Curl::CurlInterface> curl;
            std::unique_ptr<UKControllerPlugin::Windows::WinApiInterface> windows;

            // The helpers and collections
            std::unique_ptr<UKControllerPlugin::Api::ApiInterface> api;
            std::unique_ptr<UKControllerPlugin::TaskManager::TaskRunnerInterface> taskRunner;
            std::unique_ptr<UKControllerPlugin::Controller::ActiveCallsignCollection> activeCallsigns;
            std::unique_ptr<UKControllerPlugin::Flightplan::StoredFlightplanCollection> flightplans;
            std::unique_ptr<UKControllerPlugin::Message::UserMessager> userMessager;
            std::unique_ptr<UKControllerPlugin::Euroscope::UserSetting> pluginUserSettingHandler;
            std::shared_ptr<UKControllerPlugin::Controller::Login> login;
            std::unique_ptr<UKControllerPlugin::Dialog::DialogManager> dialogManager;
            std::unique_ptr<UKControllerPlugin::Setting::SettingRepository> settingsRepository;
            std::shared_ptr<UKControllerPlugin::Datablock::DisplayTime> timeFormatting;

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
            std::unique_ptr<UKControllerPlugin::UKPlugin> plugin;

            // The modules
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
            std::unique_ptr<UKControllerPlugin::Handoff::HandoffCollection> handoffs;

            // Collections that are spawned multiple times.
            std::vector<std::shared_ptr<UKControllerPlugin::RadarScreen::RadarRenderableCollection>> allRadarRenders;
            std::unique_ptr<UKControllerPlugin::Plugin::FunctionCallEventHandler> pluginFunctionHandlers;

            // Graphics things
            std::unique_ptr<UKControllerPlugin::Windows::GdiplusBrushes> brushes;
            std::unique_ptr<UKControllerPlugin::Windows::GdiGraphicsWrapper> graphics;

            // Large collections that we don't want to go onto the stack
            std::unique_ptr<Sid::SidCollection> sids;
            std::unique_ptr<const UKControllerPlugin::Airfield::AirfieldCollection> airfields;
            std::unique_ptr<UKControllerPlugin::Ownership::AirfieldOwnershipManager> airfieldOwnership;
            std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionCollection> controllerPositions;
            std::unique_ptr<UKControllerPlugin::IntentionCode::SectorExitRepository> sectorExitPoints;
            std::shared_ptr<UKControllerPlugin::SectorFile::RunwayCollection> runways;
            std::shared_ptr<UKControllerPlugin::Navaids::NavaidCollection> navaids;
            std::shared_ptr<UKControllerPlugin::Hold::PublishedHoldCollection> publishedHolds;

            // Push events
            std::shared_ptr<Push::PushEventProcessorCollection> pushEventProcessors;

        } PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
