#pragma once
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/UserSettingProviderInterface.h"
#include "radarscreen/RadarScreenFactory.h"
#include "sectorfile/SectorFileProviderInterface.h"
#include "euroscope/RunwayDialogAwareCollection.h"

// Forward Declarations
namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
    namespace Euroscope {
        class EuroScopeCRadarTargetInterface;
        class EuroScopeCFlightPlanInterface;
        class AsrEventHandlerCollection;
        class RadarTargetEventHandlerCollection;
        class RadarTargetEventHandlerInterface;
    }  // namespace Euroscope
    namespace TaskManager {
        class TaskData;
        class TaskRunner;
    }  // namespace TaskManager
    namespace Controller {
        class ControllerStatusEventHandlerCollection;
        class HandoffEventHandlerCollection;
    }  // namespace Controller
    namespace TimedEvent {
        class TimedEventCollection;
    }  // namespace TimedEvent
    namespace Metar {
        class MetarEventHandlerCollection;
    }  // namespace Metar
    namespace Flightplan {
        class FlightPlanEventHandlerCollection;
    }  // namespace Flightplan
    namespace Tag {
        class TagItemCollection;
    }  // namespace Tag
    namespace Plugin {
        class FunctionCallEventHandler;
    }  // namespace Plugin
    namespace Command {
        class CommandHandlerCollection;
    }  // namespace Command

    class UKRadarScreen;
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {

    /*
        This is the main plugin class. It is main class that represents the plugin
        in Euroscope. It handles loading different modules, providing Euroscope
        instances of Radar Screens, as well as handling basic Euroscope commands
        and events.
    */
    class UKPlugin : public EuroScopePlugIn::CPlugIn,
        public UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface,
        public UKControllerPlugin::Euroscope::UserSettingProviderInterface,
        public UKControllerPlugin::SectorFile::SectorFileProviderInterface
    {
        public:
            UKPlugin(
                const UKControllerPlugin::Euroscope::RadarTargetEventHandlerCollection & radarTargetEventHandler,
                const UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection & flightplanEventHandler,
                const UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection & statusEventHandler,
                const UKControllerPlugin::TimedEvent::TimedEventCollection & timedEvents,
                const UKControllerPlugin::Tag::TagItemCollection & tagEvents,
                const UKControllerPlugin::RadarScreen::RadarScreenFactory & radarScreenFactory,
                const UKControllerPlugin::Metar::MetarEventHandlerCollection & metarHandlers,
                const UKControllerPlugin::Plugin::FunctionCallEventHandler & functionCallHandler,
                const UKControllerPlugin::Command::CommandHandlerCollection & commandHandlers,
                const UKControllerPlugin::Euroscope::RunwayDialogAwareCollection & runwayDialogHandlers,
                const UKControllerPlugin::Controller::HandoffEventHandlerCollection & controllerHandoffHandlers
            );
            void AddItemToPopupList(const UKControllerPlugin::Plugin::PopupMenuItem item) override;
            void ChatAreaMessage(
                std::string handler,
                std::string sender,
                std::string message,
                bool showHandler,
                bool markUnread,
                bool overrideBusy,
                bool flash,
                bool confirm
            ) override;
            double GetDistanceFromUserVisibilityCentre(EuroScopePlugIn::CPosition position) const;
            int GetEuroscopeConnectionStatus(void) const;
            std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCControllerInterface> GetUserControllerObject(
                void
            ) const override;
            std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface> GetFlightplanForCallsign(
                std::string callsign
            ) const;
            std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface> GetRadarTargetForCallsign(
                std::string callsign
            ) const;
            std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>
                GetSelectedFlightplan()const override;
            std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>
                GetSelectedRadarTarget() const override;
            bool OnCompileCommand ( const char * sCommandLine );
            void OnControllerDisconnect(EuroScopePlugIn::CController Controller);
            void OnControllerPositionUpdate(EuroScopePlugIn::CController Controller);
            void OnFlightPlanControllerAssignedDataUpdate(EuroScopePlugIn::CFlightPlan flightPlan, int dataType);
            void OnFlightPlanFlightPlanDataUpdate(EuroScopePlugIn::CFlightPlan flightPlan);
            void OnFlightPlanDisconnect(EuroScopePlugIn::CFlightPlan flightPlan);
            void OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area);
            void OnGetTagItem(
                EuroScopePlugIn::CFlightPlan FlightPlan,
                EuroScopePlugIn::CRadarTarget RadarTarget,
                int ItemCode,
                int TagData,
                char sItemString[16],
                int * pColorCode,
                COLORREF * pRGB,
                double * pFontSize
            );
            void OnNewMetarReceived(const char * sStation, const char * sFullMetar);
            void OnAirportRunwayActivityChanged(void) override;
            void OnTimer(int time);
            inline virtual EuroScopePlugIn::CRadarScreen * OnRadarScreenCreated(
                const char * sDisplayName,
                bool NeedRadarContent,
                bool GeoReferenced,
                bool CanBeSaved,
                bool CanBeCreated
            );
            void OnRadarTargetPositionUpdate(EuroScopePlugIn::CRadarTarget radarTarget);
            void TriggerPopupList(RECT area, std::string title, int numColumns) override;
            void PostInit(void);
            void RegisterTagFunction(int itemCode, std::string description) override;
            void RegisterTagItem(int itemCode, std::string description);
            void TriggerFlightplanUpdateForCallsign(std::string callsign);
            void ApplyFunctionToAllFlightplans(
                std::function<
                void(
                std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>,
                std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>
            )
                > function
            ) override;
            void ShowTextEditPopup(RECT editArea, int callbackId, std::string initialValue) override;
            void OnFlightPlanFlightStripPushed(
                EuroScopePlugIn::CFlightPlan flightplan,
                const char* sendingController,
                const char* targetController
            ) override;

            // Inherited via UserSettingProviderInterface
            std::string GetKey(std::string key) override;
            bool KeyExists(std::string key) override;
            void SetKey(std::string key, std::string description, std::string value) override;

            // Inherited via SectorFileProviderInterface
            std::set<std::shared_ptr<UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface>>
                GetAllElementsByType(int type) override;

        private:

            bool ControllerIsMe(EuroScopePlugIn::CController controller, EuroScopePlugIn::CController me);
            void DoInitialControllerLoad(void);
            void DoInitialFlightplanLoad(void);

            // An event handler for RadarTarget events
            const UKControllerPlugin::Euroscope::RadarTargetEventHandlerCollection & radarTargetEventHandler;

            // An event handler for FlightPlan events
            const UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection & flightplanEventHandler;

            // An event handler for controller status
            const UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection & statusEventHandler;

            // Timed events
            const UKControllerPlugin::TimedEvent::TimedEventCollection & timedEvents;

            // Factory for creating radar screens
            const UKControllerPlugin::RadarScreen::RadarScreenFactory radarScreenFactory;

            // Events involving tag items
            const UKControllerPlugin::Tag::TagItemCollection & tagEvents;

            // Collection of handlers for METAR events
            const UKControllerPlugin::Metar::MetarEventHandlerCollection & metarHandlers;

            // Handler for function calls
            const UKControllerPlugin::Plugin::FunctionCallEventHandler & functionCallHandler;

            // Handles user dot commands
            const UKControllerPlugin::Command::CommandHandlerCollection & commandHandlers;

            // Handles runways changing activity
            const UKControllerPlugin::Euroscope::RunwayDialogAwareCollection & runwayDialogHandlers;

            // Handles handoffs between controllers
            const UKControllerPlugin::Controller::HandoffEventHandlerCollection& controllerHandoffHandlers;

            // Whether or not we've initialised the plugin.
            bool initialised = false;
};  // namespace Windows
}  // namespace UKControllerPlugin
