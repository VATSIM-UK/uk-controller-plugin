#include "pch/stdafx.h"
#include "plugin/UKPlugin.h"
#include "radarscreen/UKRadarScreen.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "euroscope/EuroScopeCRadarTargetWrapper.h"
#include "euroscope/EuroScopeCFlightPlanWrapper.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "euroscope/EuroScopeCControllerWrapper.h"
#include "timedevent/TimedEventCollection.h"
#include "tag/TagItemCollection.h"
#include "metar/MetarEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "update/PluginVersion.h"
#include "command/CommandHandlerCollection.h"

using UKControllerPlugin::TaskManager::TaskRunner;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetWrapper;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanWrapper;
using UKControllerPlugin::Euroscope::RadarTargetEventHandlerCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Euroscope::EuroScopeCControllerWrapper;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::RadarScreen::RadarScreenFactory;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PluginVersion;
using UKControllerPlugin::Command::CommandHandlerCollection;

namespace UKControllerPlugin {

    UKPlugin::UKPlugin(
        const RadarTargetEventHandlerCollection & radarTargetEventHandler,
        const FlightPlanEventHandlerCollection & flightplanEventHandler,
        const ControllerStatusEventHandlerCollection & statusEventHandler,
        const TimedEventCollection & timedEvents,
        const TagItemCollection & tagEvents,
        const RadarScreenFactory & radarScreenFactory,
        const MetarEventHandlerCollection & metarHandlers,
        const FunctionCallEventHandler & functionCallHandler,
        const CommandHandlerCollection & commandHandlers
    )
        : UKPlugin::CPlugIn(
            EuroScopePlugIn::COMPATIBILITY_CODE,
            PluginVersion::title,
            PluginVersion::version,
            PluginVersion::author,
            PluginVersion::copyright
        ),
        timedEvents(timedEvents),
        statusEventHandler(statusEventHandler),
        radarTargetEventHandler(radarTargetEventHandler),
        flightplanEventHandler(flightplanEventHandler),
        tagEvents(tagEvents),
        radarScreenFactory(radarScreenFactory),
        metarHandlers(metarHandlers),
        functionCallHandler(functionCallHandler),
        commandHandlers(commandHandlers)
    {
        this->DisplayUserMessage(
            "message",
            "UKCP",
            "UK Controller Plugin Loaded Successfully",
            false,
            false,
            false,
            false,
            false
        );
    }

    void UKPlugin::ChatAreaMessage(
        std::string handler,
        std::string sender,
        std::string message,
        bool showHandler,
        bool markUnread,
        bool overrideBusy,
        bool flash,
        bool confirm
    ) {
        this->DisplayUserMessage(
            handler.c_str(),
            sender.c_str(),
            message.c_str(),
            showHandler,
            markUnread,
            overrideBusy,
            flash,
            confirm
        );
    }

    double UKPlugin::GetDistanceFromUserVisibilityCentre(EuroScopePlugIn::CPosition position) const
    {
        return this->ControllerMyself().GetPosition().DistanceTo(position);
    }

    /*
        Returns true if the two controllers are deemed to be the same person.
    */
    bool UKPlugin::ControllerIsMe(EuroScopePlugIn::CController controller, EuroScopePlugIn::CController me)
    {
        return controller.IsValid() &&
            strcmp(controller.GetFullName(), me.GetFullName()) == 0 &&
            controller.GetCallsign() == me.GetCallsign();
    }

    /*
        Loads all controllers on startup - incase the plugin gets initialised
        with Euroscope already open.
    */
    void UKPlugin::DoInitialControllerLoad(void)
    {
        EuroScopePlugIn::CController current = this->ControllerSelectFirst();

        // If there's nobody online, stop.
        if (strcmp(current.GetCallsign(), "") == 0) {
            return;
        }

        // Loop through all visible controllers
        do {
            this->OnControllerPositionUpdate(current);
        } while (strcmp((current = this->ControllerSelectNext(current)).GetCallsign(), "") != 0);
    }

    /*
        Loads the flightplans on startup - incase the plugin gets initialised with
        Euroscope already open.
    */
    void UKPlugin::DoInitialFlightplanLoad(void)
    {
        EuroScopePlugIn::CFlightPlan current = this->FlightPlanSelectFirst();

        // If there's nobody online, stop.
        if (strcmp(current.GetCallsign(), "") == 0) {
            return;
        }

        // Loop through all visible controllers
        do {
            this->OnFlightPlanFlightPlanDataUpdate(current);
        } while (strcmp((current = this->FlightPlanSelectNext(current)).GetCallsign(), "") != 0);
    }

    /*
        Gets a given key from user settings.
    */
    std::string UKPlugin::GetKey(std::string key)
    {
        return this->KeyExists(key) ?  this->GetDataFromSettings(key.c_str()) : "";
    }


    /*
        Returns whether or not a key exists in user settings.
    */
    bool UKPlugin::KeyExists(std::string key)
    {
        return this->GetDataFromSettings(key.c_str()) != NULL;
    }

    /*
        Sets a value in user settings.
    */
    void UKPlugin::SetKey(std::string key, std::string description, std::string value)
    {
        this->SaveDataToSettings(key.c_str(), description.c_str(), value.c_str());
    }

    /*
        Returns the EuroScope connection status.
    */
    int UKPlugin::GetEuroscopeConnectionStatus(void) const
    {
        return this->GetConnectionType();
    }

    /*
        Gets a flightplan for a given callsign.
    */
    std::shared_ptr<EuroScopeCFlightPlanInterface> UKPlugin::GetFlightplanForCallsign(std::string callsign) const
    {
        EuroScopePlugIn::CFlightPlan plan = this->FlightPlanSelect(callsign.c_str());

        if (!plan.IsValid() || plan.GetSimulated()) {
            throw std::invalid_argument("Flightplan not found");
        }

        return std::shared_ptr<EuroScopeCFlightPlanInterface>(new EuroScopeCFlightPlanWrapper(plan));
    }

    std::shared_ptr<EuroScopeCRadarTargetInterface> UKPlugin::GetRadarTargetForCallsign(std::string callsign) const
    {
        EuroScopePlugIn::CRadarTarget target = this->RadarTargetSelect(callsign.c_str());

        if (!target.IsValid()) {
            throw std::invalid_argument("Target not found");
        }

        return std::shared_ptr<EuroScopeCRadarTargetInterface>(new EuroScopeCRadarTargetWrapper(target));
    }

    /*
        The user has entered a dot command
    */
    bool UKPlugin::OnCompileCommand(const char * command)
    {
        return this->commandHandlers.ProcessCommand(command);
    }

    /*
        Called by EuroScope when a controller position disconnects or times out.
    */
    void UKPlugin::OnControllerDisconnect(EuroScopePlugIn::CController Controller)
    {
        this->statusEventHandler.ControllerDisconnectEvent(
            EuroScopeCControllerWrapper(Controller, this->ControllerIsMe(Controller, this->ControllerMyself()))
        );
    }

    /*
        Called by EuroScope when a controller position is updated.
    */
    void UKPlugin::OnControllerPositionUpdate(EuroScopePlugIn::CController Controller)
    {
        this->statusEventHandler.ControllerUpdateEvent(
            EuroScopeCControllerWrapper(Controller, this->ControllerIsMe(Controller, this->ControllerMyself()))
        );
    }

    /*
        Called by EuroScope when a controller updates flightplanned data (e.g. direct to)
    */
    void UKPlugin::OnFlightPlanControllerAssignedDataUpdate(EuroScopePlugIn::CFlightPlan flightPlan, int dataType)
    {
        this->flightplanEventHandler.ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanWrapper(flightPlan),
            EuroScopeCRadarTargetWrapper(this->RadarTargetSelect(flightPlan.GetCallsign())),
            dataType
        );
    }

    /*
        Called by EuroScope when flightplan data is updated.
    */
    void UKPlugin::OnFlightPlanFlightPlanDataUpdate(EuroScopePlugIn::CFlightPlan flightPlan)
    {
        this->flightplanEventHandler.FlightPlanEvent(
            EuroScopeCFlightPlanWrapper(flightPlan),
            EuroScopeCRadarTargetWrapper(this->RadarTargetSelect(flightPlan.GetCallsign()))
        );
    }

    /*
        Called by Euroscope when an aircraft disconnects from the network. If the aircraft
        is NOFP, then the only really useful information we get from the argument is the callsign.
    */
    void UKPlugin::OnFlightPlanDisconnect(EuroScopePlugIn::CFlightPlan flightPlan)
    {
        this->flightplanEventHandler.FlightPlanDisconnectEvent(
            EuroScopeCFlightPlanWrapper(flightPlan)
        );
    }

    /*
        Called when a numbered Euroscope function is triggered, pass it on to the registered handler.
    */
    void UKPlugin::OnFunctionCall(int functionId, const char * sItemString, POINT Pt, RECT Area)
    {
        this->functionCallHandler.CallFunction(
            functionId,
            sItemString,
            EuroScopeCFlightPlanWrapper(this->FlightPlanSelectASEL()),
            EuroScopeCRadarTargetWrapper(this->RadarTargetSelectASEL())
        );
    }

    /*
        Handles the rendering of TAG items.
    */
    void UKPlugin::OnGetTagItem(
        EuroScopePlugIn::CFlightPlan FlightPlan,
        EuroScopePlugIn::CRadarTarget RadarTarget,
        int ItemCode,
        int TagData,
        char sItemString[16],
        int * pColorCode,
        COLORREF * pRGB,
        double * pFontSize
    ) {
        this->tagEvents.TagItemUpdate(
            ItemCode,
            sItemString,
            EuroScopeCFlightPlanWrapper(FlightPlan),
            EuroScopeCRadarTargetWrapper(RadarTarget)
        );
    }

    /*
        Called when EuroScope receives a new METAR.
    */
    void UKPlugin::OnNewMetarReceived(const char * sStation, const char * sFullMetar)
    {
        this->metarHandlers.NewMetarEvent(sStation, sFullMetar);
    }

    /*
        Returns a pointer a UKRadarScreen instance when Euroscope
        creates a radar screen.

        This happens for every new ASR file that is opened, but only
        the first time that it is loaded.
    */
    inline EuroScopePlugIn::CRadarScreen * UKPlugin::OnRadarScreenCreated(
        const char * sDisplayName,
        bool NeedRadarContent,
        bool GeoReferenced,
        bool CanBeSaved,
        bool CanBeCreated
    ) {
        return radarScreenFactory.Create();
    }

    /*
        Called by EuroScope every time a radar target has a position update. Pass to the event handler.
    */
    void UKPlugin::OnRadarTargetPositionUpdate(EuroScopePlugIn::CRadarTarget radarTarget)
    {
        this->radarTargetEventHandler.RadarTargetEvent(EuroScopeCRadarTargetWrapper(radarTarget));
    }

    /*
        Do a sweep of the login status, and go gather all the online controller information
    */
    void UKPlugin::PostInit(void)
    {
        // Only initialise once
        if (this->initialised) {
            return;
        }

        // Do initialisation
        this->DoInitialControllerLoad();
        this->DoInitialFlightplanLoad();
        this->initialised = true;
    }

    /*
        Registers a function with EuroScope.
    */
    void UKPlugin::RegisterTagFunction(int itemCode, std::string description)
    {
        this->RegisterTagItemFunction(description.c_str(), itemCode);
    }

    /*
        Registers a TAG item with Euroscope.
    */
    void UKPlugin::RegisterTagItem(int itemCode, std::string description)
    {
        this->RegisterTagItemType(description.c_str(), itemCode);
    }

    /*
        Triggers the Euroscope OnFlightplanUpdate event for a given callsign.
    */
    void UKPlugin::TriggerFlightplanUpdateForCallsign(std::string callsign)
    {
        EuroScopePlugIn::CFlightPlan plan = this->FlightPlanSelect(callsign.c_str());

        if (plan.IsValid() && !plan.GetSimulated()) {
            this->OnFlightPlanFlightPlanDataUpdate(plan);
        }
    }

    /*
        Called on a timer by EuroScope.
    */
    void UKPlugin::OnTimer(int time)
    {
        this->timedEvents.Tick(time);
    }
}  // namespace UKControllerPlugin
