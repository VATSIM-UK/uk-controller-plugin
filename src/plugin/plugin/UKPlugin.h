#pragma once
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "euroscope/UserSettingProviderInterface.h"
#include "radarscreen/RadarScreenFactory.h"
#include "sectorfile/SectorFileProviderInterface.h"

// Forward Declarations
namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
    namespace Euroscope {
        class EuroScopeCRadarTargetInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroscopeFlightplanListInterface;
        class AsrEventHandlerCollection;
        class RadarTargetEventHandlerCollection;
        class RadarTargetEventHandlerInterface;
    } // namespace Euroscope
    namespace TaskManager {
        class TaskRunner;
    } // namespace TaskManager
    namespace Controller {
        class ControllerStatusEventHandlerCollection;
        class HandoffEventHandlerCollection;
    } // namespace Controller
    namespace TimedEvent {
        class TimedEventCollection;
    } // namespace TimedEvent
    namespace Metar {
        class MetarEventHandlerCollection;
    } // namespace Metar
    namespace Flightplan {
        class FlightPlanEventHandlerCollection;
    } // namespace Flightplan
    namespace Tag {
        class TagItemCollection;
    } // namespace Tag
    namespace Plugin {
        class FunctionCallEventHandler;
    } // namespace Plugin
    namespace Command {
        class CommandHandlerCollection;
    } // namespace Command

    class UKRadarScreen;
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {

    /*
        This is the main plugin class. It is main class that represents the plugin
        in Euroscope. It handles loading different modules, providing Euroscope
        instances of Radar Screens, as well as handling basic Euroscope commands
        and events.
    */
    class UKPlugin : public EuroScopePlugIn::CPlugIn,
                     public Euroscope::EuroscopePluginLoopbackInterface,
                     public Euroscope::UserSettingProviderInterface,
                     public SectorFile::SectorFileProviderInterface
    {
        public:
        UKPlugin(
            const Euroscope::RadarTargetEventHandlerCollection& radarTargetEventHandler,
            const Flightplan::FlightPlanEventHandlerCollection& flightplanEventHandler,
            const Controller::ControllerStatusEventHandlerCollection& statusEventHandler,
            const TimedEvent::TimedEventCollection& timedEvents,
            const Tag::TagItemCollection& tagEvents,
            RadarScreen::RadarScreenFactory radarScreenFactory,
            const Metar::MetarEventHandlerCollection& metarHandlers,
            const Plugin::FunctionCallEventHandler& functionCallHandler,
            const Command::CommandHandlerCollection& commandHandlers,
            const Euroscope::RunwayDialogAwareCollection& runwayDialogHandlers,
            const Controller::HandoffEventHandlerCollection& controllerHandoffHandlers);
        void AddItemToPopupList(Plugin::PopupMenuItem item) override;
        void ChatAreaMessage(
            std::string handler,
            std::string sender,
            std::string message,
            bool showHandler,
            bool markUnread,
            bool overrideBusy,
            bool flash,
            bool confirm) override;
        auto GetDistanceFromUserVisibilityCentre(EuroScopePlugIn::CPosition position) const -> double override;
        auto GetEuroscopeConnectionStatus() const -> int override;
        auto GetUserControllerObject() const -> std::shared_ptr<Euroscope::EuroScopeCControllerInterface> override;
        auto GetFlightplanForCallsign(std::string callsign) const
            -> std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface> override;
        auto GetRadarTargetForCallsign(std::string callsign) const
            -> std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface> override;
        auto GetSelectedFlightplan() const -> std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface> override;
        auto GetSelectedRadarTarget() const -> std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface> override;
        auto OnCompileCommand(const char* sCommandLine) -> bool override;
        void OnControllerDisconnect(EuroScopePlugIn::CController Controller) override;
        void OnControllerPositionUpdate(EuroScopePlugIn::CController Controller) override;
        void OnFlightPlanControllerAssignedDataUpdate(EuroScopePlugIn::CFlightPlan flightPlan, int dataType) override;
        void OnFlightPlanFlightPlanDataUpdate(EuroScopePlugIn::CFlightPlan flightPlan) override;
        void OnFlightPlanDisconnect(EuroScopePlugIn::CFlightPlan flightPlan) override;
        void OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area) override;
        void OnGetTagItem(
            EuroScopePlugIn::CFlightPlan FlightPlan,
            EuroScopePlugIn::CRadarTarget RadarTarget,
            int ItemCode,
            int TagData,
            char sItemString[16], // NOLINT
            int* pColorCode,
            COLORREF* pRGB,
            double* pFontSize) override;
        void OnNewMetarReceived(const char* sStation, const char* sFullMetar) override;
        void OnAirportRunwayActivityChanged() override;
        void OnTimer(int time) override;
        inline auto OnRadarScreenCreated(
            const char* sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated)
            -> EuroScopePlugIn::CRadarScreen* override;
        void OnRadarTargetPositionUpdate(EuroScopePlugIn::CRadarTarget radarTarget) override;
        void TriggerPopupList(RECT area, std::string title, int numColumns) override;
        void PostInit();
        void RegisterTagFunction(int itemCode, std::string description) override;
        void RegisterTagItem(int itemCode, std::string description) override;
        void TriggerFlightplanUpdateForCallsign(std::string callsign) override;
        void ApplyFunctionToAllFlightplans(
            std::function<void(
                std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface>,
                std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface>)> function) override;
        void ShowTextEditPopup(RECT editArea, int callbackId, std::string initialValue) override;
        void OnFlightPlanFlightStripPushed(
            EuroScopePlugIn::CFlightPlan flightplan,
            const char* sendingController,
            const char* targetController) override;
        void
        SetEuroscopeSelectedFlightplan(std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface> flightplan) override;
        void SetEuroscopeSelectedFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) override;

        // Inherited via UserSettingProviderInterface
        auto GetKey(std::string key) -> std::string override;
        auto KeyExists(std::string key) -> bool override;
        void SetKey(std::string key, std::string description, std::string value) override;

        // Inherited via SectorFileProviderInterface
        auto GetAllElementsByType(int type)
            -> std::set<std::shared_ptr<Euroscope::EuroscopeSectorFileElementInterface>> override;

        void ApplyFunctionToAllControllers(
            std::function<void(std::shared_ptr<Euroscope::EuroScopeCControllerInterface>)> function) override;
        auto RegisterFlightplanList(std::string name)
            -> std::shared_ptr<Euroscope::EuroscopeFlightplanListInterface> override;

        private:
        static auto ControllerIsMe(EuroScopePlugIn::CController controller, EuroScopePlugIn::CController me) -> bool;
        void DoInitialControllerLoad();
        void DoInitialFlightplanLoad();

        // An event handler for RadarTarget events
        const Euroscope::RadarTargetEventHandlerCollection& radarTargetEventHandler;

        // An event handler for FlightPlan events
        const Flightplan::FlightPlanEventHandlerCollection& flightplanEventHandler;

        // An event handler for controller status
        const Controller::ControllerStatusEventHandlerCollection& statusEventHandler;

        // Timed events
        const TimedEvent::TimedEventCollection& timedEvents;

        // Factory for creating radar screens
        const RadarScreen::RadarScreenFactory radarScreenFactory;

        // Events involving tag items
        const Tag::TagItemCollection& tagEvents;

        // Collection of handlers for METAR events
        const Metar::MetarEventHandlerCollection& metarHandlers;

        // Handler for function calls
        const Plugin::FunctionCallEventHandler& functionCallHandler;

        // Handles user dot commands
        const Command::CommandHandlerCollection& commandHandlers;

        // Handles runways changing activity
        const Euroscope::RunwayDialogAwareCollection& runwayDialogHandlers;

        // Handles handoffs between controllers
        const Controller::HandoffEventHandlerCollection& controllerHandoffHandlers;

        // Whether or not we've initialised the plugin.
        bool initialised = false;
    }; // namespace Windows
} // namespace UKControllerPlugin
