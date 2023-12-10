#include "FunctionCallEventHandler.h"
#include "UKPlugin.h"
#include "command/CommandHandlerCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "euroscope/EuroScopeCControllerWrapper.h"
#include "euroscope/EuroScopeCFlightPlanWrapper.h"
#include "euroscope/EuroScopeCRadarTargetWrapper.h"
#include "euroscope/EuroscopeFlightplanListInterface.h"
#include "euroscope/EuroscopeFlightplanListWrapper.h"
#include "euroscope/EuroscopeSectorFileElementWrapper.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "radarscreen/UKRadarScreen.h"
#include "tag/TagData.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "update/PluginVersion.h"

using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCControllerWrapper;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanWrapper;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetWrapper;
using UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface;
using UKControllerPlugin::Euroscope::EuroscopeFlightplanListWrapper;
using UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface;
using UKControllerPlugin::Euroscope::EuroscopeSectorFileElementWrapper;
using UKControllerPlugin::Euroscope::RadarTargetEventHandlerCollection;
using UKControllerPlugin::Euroscope::RunwayDialogAwareCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PluginVersion;
using UKControllerPlugin::RadarScreen::RadarScreenFactory;
using UKControllerPlugin::Tag::TagData;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::TaskManager::TaskRunner;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {

    UKPlugin::UKPlugin(
        const RadarTargetEventHandlerCollection& radarTargetEventHandler,
        const FlightPlanEventHandlerCollection& flightplanEventHandler,
        const ControllerStatusEventHandlerCollection& statusEventHandler,
        const TimedEventCollection& timedEvents,
        const TagItemCollection& tagEvents,
        RadarScreenFactory radarScreenFactory,
        const FunctionCallEventHandler& functionCallHandler,
        const CommandHandlerCollection& commandHandlers,
        const RunwayDialogAwareCollection& runwayDialogHandlers,
        const HandoffEventHandlerCollection& controllerHandoffHandlers)
        : UKPlugin::CPlugIn(
              EuroScopePlugIn::COMPATIBILITY_CODE,
              PluginVersion::title,
              PluginVersion::version,
              PluginVersion::author,
              PluginVersion::copyright),
          radarTargetEventHandler(radarTargetEventHandler), flightplanEventHandler(flightplanEventHandler),
          statusEventHandler(statusEventHandler), timedEvents(timedEvents),
          radarScreenFactory(std::move(radarScreenFactory)), tagEvents(tagEvents),
          functionCallHandler(functionCallHandler), commandHandlers(commandHandlers),
          runwayDialogHandlers(runwayDialogHandlers), controllerHandoffHandlers(controllerHandoffHandlers)

    {
    }

    /*
        Add an item to an already open popup list
    */
    void UKPlugin::AddItemToPopupList(const UKControllerPlugin::Plugin::PopupMenuItem item)
    {
        AddPopupListElement(
            item.firstValue.c_str(),
            item.secondValue.c_str(),
            item.callbackFunctionId,
            false,
            item.checked,
            item.disabled,
            item.fixedPosition);
    }

    void UKPlugin::ChatAreaMessage(
        std::string handler,
        std::string sender,
        std::string message,
        bool showHandler,
        bool markUnread,
        bool overrideBusy,
        bool flash,
        bool confirm)
    {
        this->DisplayUserMessage(
            handler.c_str(), sender.c_str(), message.c_str(), showHandler, markUnread, overrideBusy, flash, confirm);
    }

    auto UKPlugin::GetDistanceFromUserVisibilityCentre(EuroScopePlugIn::CPosition position) const -> double
    {
        return this->ControllerMyself().GetPosition().DistanceTo(position);
    }

    /*
        Returns true if the two controllers are deemed to be the same person.
    */
    auto UKPlugin::ControllerIsMe(EuroScopePlugIn::CController controller, EuroScopePlugIn::CController me) -> bool
    {
        return controller.IsValid() && strcmp(controller.GetFullName(), me.GetFullName()) == 0 &&
               controller.GetCallsign() == me.GetCallsign();
    }

    /*
        Loads all controllers on startup - incase the plugin gets initialised
        with Euroscope already open.
    */
    void UKPlugin::DoInitialControllerLoad()
    {
        LogInfo("Initial controller load started");

        EuroScopePlugIn::CController me = this->ControllerMyself();
        if (me.IsValid()) {
            this->OnControllerPositionUpdate(me);
            LogInfo("Loaded myself, on " + std::string(me.GetCallsign()));
        }

        EuroScopePlugIn::CController current = this->ControllerSelectFirst();

        // If there's nobody online, stop.
        if (strcmp(current.GetCallsign(), "") == 0) {
            LogInfo("Initial controller load complete, none found");
            return;
        }

        // Loop through all visible controllers
        do {
            if (!current.IsValid()) {
                continue;
            }

            this->OnControllerPositionUpdate(current);
        } while (strcmp((current = this->ControllerSelectNext(current)).GetCallsign(), "") != 0);
        LogInfo("Initial controller load complete");
    }

    /*
        Loads the flightplans on startup - incase the plugin gets initialised with
        Euroscope already open.
    */
    void UKPlugin::DoInitialFlightplanLoad()
    {
        LogInfo("Initial fightplan load started");
        EuroScopePlugIn::CFlightPlan current = this->FlightPlanSelectFirst();

        // If there's nobody online, stop.
        if (!current.IsValid() || strcmp(current.GetCallsign(), "") == 0) {
            LogInfo("Initial fightplan load complete, none found");
            return;
        }

        // Loop through all visible flightplans
        do {
            if (!current.IsValid()) {
                continue;
            }

            this->OnFlightPlanFlightPlanDataUpdate(current);
        } while (strcmp((current = this->FlightPlanSelectNext(current)).GetCallsign(), "") != 0);
        LogInfo("Initial fightplan load complete");
    }

    /*
        Get all the sector file elements by type
    */
    auto UKPlugin::GetAllElementsByType(int type) -> std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>>
    {
        this->SelectActiveSectorfile();
        EuroScopePlugIn::CSectorElement selected = this->SectorFileElementSelectFirst(type);

        std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>> elements;

        while (true) {
            if (!selected.IsValid()) {
                break;
            }

            elements.insert(std::make_shared<EuroscopeSectorFileElementWrapper>(selected));
            selected = this->SectorFileElementSelectNext(selected, type);
        }

        return elements;
    }

    void UKPlugin::ApplyFunctionToAllControllers(
        std::function<void(std::shared_ptr<EuroScopeCControllerInterface>)> function)
    {
        EuroScopePlugIn::CController current = this->ControllerSelectFirst();

        // If there's nothing, stop
        if (!current.IsValid() || strcmp(current.GetCallsign(), "") == 0) {
            return;
        }

        // Loop through all visible controllers
        do {
            if (!current.IsValid()) {
                continue;
            }

            function(std::make_shared<EuroScopeCControllerWrapper>(
                current, this->ControllerIsMe(current, this->ControllerMyself())));
        } while (strcmp((current = this->ControllerSelectNext(current)).GetCallsign(), "") != 0);
    }

    auto UKPlugin::RegisterFlightplanList(std::string name) -> std::shared_ptr<EuroscopeFlightplanListInterface>
    {
        EuroScopePlugIn::CFlightPlanList list = this->RegisterFpList(name.c_str());
        return std::make_shared<EuroscopeFlightplanListWrapper>(list);
    }

    /*
        Gets a given key from user settings.
    */
    auto UKPlugin::GetKey(std::string key) -> std::string
    {
        return this->KeyExists(key) ? this->GetDataFromSettings(key.c_str()) : "";
    }

    /*
        Returns whether or not a key exists in user settings.
    */
    auto UKPlugin::KeyExists(std::string key) -> bool
    {
        return this->GetDataFromSettings(key.c_str()) != nullptr;
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
    auto UKPlugin::GetEuroscopeConnectionStatus() const -> int
    {
        return this->GetConnectionType();
    }

    /*
        Return the controller object related to the user that is logged in.
    */
    auto UKPlugin::GetUserControllerObject() const -> std::shared_ptr<EuroScopeCControllerInterface>
    {
        EuroScopePlugIn::CController me = this->ControllerMyself();

        if (!me.IsValid()) {
            return nullptr;
        }

        return std::make_shared<EuroScopeCControllerWrapper>(me, true);
    }

    /*
        Gets a flightplan for a given callsign.
    */
    auto UKPlugin::GetFlightplanForCallsign(std::string callsign) const
        -> std::shared_ptr<EuroScopeCFlightPlanInterface>
    {
        EuroScopePlugIn::CFlightPlan plan = this->FlightPlanSelect(callsign.c_str());

        if (!plan.IsValid()) {
            return nullptr;
        }

        return std::make_shared<EuroScopeCFlightPlanWrapper>(plan);
    }

    /*
        Gets a flightplan for a given callsign.
    */
    auto UKPlugin::GetRadarTargetForCallsign(std::string callsign) const
        -> std::shared_ptr<EuroScopeCRadarTargetInterface>
    {
        EuroScopePlugIn::CRadarTarget target = this->RadarTargetSelect(callsign.c_str());

        if (!target.IsValid()) {
            return nullptr;
        }

        return std::make_shared<EuroScopeCRadarTargetWrapper>(target);
    }

    /*
        Returns the currently selected flightplan
    */
    auto UKPlugin::GetSelectedFlightplan() const -> std::shared_ptr<EuroScopeCFlightPlanInterface>
    {
        EuroScopePlugIn::CFlightPlan fp = this->FlightPlanSelectASEL();

        if (!fp.IsValid()) {
            return nullptr;
        }

        return std::make_shared<EuroScopeCFlightPlanWrapper>(fp);
    }

    /*
        Returns the currently selected radar target
    */
    auto UKPlugin::GetSelectedRadarTarget() const -> std::shared_ptr<EuroScopeCRadarTargetInterface>
    {
        EuroScopePlugIn::CRadarTarget rt = this->RadarTargetSelectASEL();

        if (!rt.IsValid()) {
            return nullptr;
        }

        return std::make_shared<EuroScopeCRadarTargetWrapper>(rt);
    }

    /*
        The user has entered a dot command
    */
    auto UKPlugin::OnCompileCommand(const char* command) -> bool
    {
        return this->commandHandlers.ProcessCommand(command);
    }

    /*
        Called by EuroScope when a controller position disconnects or times out.
    */
    void UKPlugin::OnControllerDisconnect(EuroScopePlugIn::CController Controller)
    {
        if (!Controller.IsValid()) {
            return;
        }

        EuroScopeCControllerWrapper wrapper(Controller, this->ControllerIsMe(Controller, this->ControllerMyself()));
        this->statusEventHandler.ControllerDisconnectEvent(wrapper);
    }

    /*
        Called by EuroScope when a controller position is updated.
    */
    void UKPlugin::OnControllerPositionUpdate(EuroScopePlugIn::CController Controller)
    {
        if (!Controller.IsValid()) {
            return;
        }

        EuroScopeCControllerWrapper wrapper(Controller, this->ControllerIsMe(Controller, this->ControllerMyself()));
        this->statusEventHandler.ControllerUpdateEvent(wrapper);
    }

    /*
        Called by EuroScope when a controller updates flightplanned data (e.g. direct to)
    */
    void UKPlugin::OnFlightPlanControllerAssignedDataUpdate(EuroScopePlugIn::CFlightPlan flightPlan, int dataType)
    {
        if (!flightPlan.IsValid()) {
            return;
        }

        EuroScopeCFlightPlanWrapper flightplanWrapper(flightPlan);
        EuroScopeCRadarTargetWrapper radarTargetWrapper(this->RadarTargetSelect(flightPlan.GetCallsign()));
        this->flightplanEventHandler.ControllerFlightPlanDataEvent(flightplanWrapper, radarTargetWrapper, dataType);
    }

    /*
        Called by EuroScope when flightplan data is updated.
    */
    void UKPlugin::OnFlightPlanFlightPlanDataUpdate(EuroScopePlugIn::CFlightPlan flightPlan)
    {
        if (!flightPlan.IsValid()) {
            return;
        }

        EuroScopeCFlightPlanWrapper flightplanWrapper(flightPlan);
        EuroScopeCRadarTargetWrapper radarTargetWrapper(this->RadarTargetSelect(flightPlan.GetCallsign()));
        this->flightplanEventHandler.FlightPlanEvent(flightplanWrapper, radarTargetWrapper);
    }

    /*
        Called by Euroscope when an aircraft disconnects from the network. If the aircraft
        is NOFP, then the only really useful information we get from the argument is the callsign.
    */
    void UKPlugin::OnFlightPlanDisconnect(EuroScopePlugIn::CFlightPlan flightPlan)
    {
        EuroScopeCFlightPlanWrapper flightplanWrapper(flightPlan);
        this->flightplanEventHandler.FlightPlanDisconnectEvent(flightplanWrapper);
    }

    /*
        Called when a numbered Euroscope function is triggered, pass it on to the registered handler.
    */
    void UKPlugin::OnFunctionCall(int functionId, const char* sItemString, POINT Pt, RECT Area)
    {
        auto flightplan = EuroScopeCFlightPlanWrapper(this->FlightPlanSelectASEL());
        auto radarTarget = EuroScopeCRadarTargetWrapper(this->RadarTargetSelectASEL());
        this->functionCallHandler.CallFunction(functionId, sItemString, flightplan, radarTarget, Pt, Area);
    }

    /*
        Handles the rendering of TAG items.
    */
    void UKPlugin::OnGetTagItem(
        EuroScopePlugIn::CFlightPlan FlightPlan,
        EuroScopePlugIn::CRadarTarget RadarTarget,
        int ItemCode,
        int dataAvailable,
        char sItemString[16], // NOLINT
        int* pColorCode,
        COLORREF* pRGB,
        double* pFontSize)
    {
        if (!FlightPlan.IsValid()) {
            return;
        }

        EuroScopeCFlightPlanWrapper flightplanWrapper(FlightPlan);
        EuroScopeCRadarTargetWrapper radarTargetWrapper(RadarTarget);
        TagData tagData(
            flightplanWrapper, radarTargetWrapper, ItemCode, dataAvailable, sItemString, pColorCode, pRGB, pFontSize);

        try {
            this->tagEvents.TagItemUpdate(tagData);
        } catch (const std::exception& e) {
            LogFatalExceptionAndRethrow("UKPlugin::OnGetTagItem::" + std::to_string(ItemCode), e);
        }
    }

    /*
        Called when someone clicks OK on the runway settings dialog.
    */
    void UKPlugin::OnAirportRunwayActivityChanged()
    {
        this->runwayDialogHandlers.RunwayDialogSaved();
    }

    /*
        Returns a pointer a UKRadarScreen instance when Euroscope
        creates a radar screen.

        This happens for every new ASR file that is opened, but only
        the first time that it is loaded.
    */
    inline auto UKPlugin::OnRadarScreenCreated(
        [[maybe_unused]] const char* sDisplayName,
        [[maybe_unused]] bool NeedRadarContent,
        [[maybe_unused]] bool GeoReferenced,
        [[maybe_unused]] bool CanBeSaved,
        [[maybe_unused]] bool CanBeCreated) -> EuroScopePlugIn::CRadarScreen*
    {
        return radarScreenFactory.Create();
    }

    /*
        Called by EuroScope every time a radar target has a position update. Pass to the event handler.
    */
    void UKPlugin::OnRadarTargetPositionUpdate(EuroScopePlugIn::CRadarTarget radarTarget)
    {
        if (!radarTarget.IsValid()) {
            return;
        }

        EuroScopeCRadarTargetWrapper radarTargetWrapper(radarTarget);
        this->radarTargetEventHandler.RadarTargetEvent(radarTargetWrapper);
    }

    /*
        Open a popup list
    */
    void UKPlugin::TriggerPopupList(RECT area, std::string title, int numColumns)
    {
        this->OpenPopupList(area, title.c_str(), numColumns);
    }

    /*
        Do a sweep of the login status, and go gather all the online controller information
    */
    void UKPlugin::PostInit()
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

        if (plan.IsValid()) {
            this->OnFlightPlanFlightPlanDataUpdate(plan);
        }
    }

    void UKPlugin::ApplyFunctionToAllFlightplans(
        std::function<void(
            std::shared_ptr<EuroScopeCFlightPlanInterface>, std::shared_ptr<EuroScopeCRadarTargetInterface>)> function)
    {
        EuroScopePlugIn::CFlightPlan current = this->FlightPlanSelectFirst();

        // If there's nothing, stop
        if (!current.IsValid() || strcmp(current.GetCallsign(), "") == 0) {
            return;
        }

        // Loop through all visible flightplans
        do {
            if (!current.IsValid()) {
                continue;
            }

            EuroScopePlugIn::CRadarTarget rt = this->RadarTargetSelect(current.GetCallsign());

            if (!rt.IsValid()) {
                continue;
            }

            function(
                std::make_shared<EuroScopeCFlightPlanWrapper>(current),
                std::make_shared<EuroScopeCRadarTargetWrapper>(rt));

        } while (strcmp((current = this->FlightPlanSelectNext(current)).GetCallsign(), "") != 0);
    }

    void UKPlugin::ApplyFunctionToAllFlightplans(
        std::function<void(EuroScopeCFlightPlanInterface&, EuroScopeCRadarTargetInterface&)> function)
    {
        EuroScopePlugIn::CFlightPlan current = this->FlightPlanSelectFirst();

        // If there's nothing, stop
        if (!current.IsValid() || strcmp(current.GetCallsign(), "") == 0) {
            return;
        }

        // Loop through all visible flightplans
        do {
            if (!current.IsValid()) {
                continue;
            }

            EuroScopePlugIn::CRadarTarget rt = this->RadarTargetSelect(current.GetCallsign());

            if (!rt.IsValid()) {
                continue;
            }

            auto flightplanWrapper = EuroScopeCFlightPlanWrapper(current);
            auto radarTargetWrapper = EuroScopeCRadarTargetWrapper(rt);
            function(flightplanWrapper, radarTargetWrapper);

        } while (strcmp((current = this->FlightPlanSelectNext(current)).GetCallsign(), "") != 0);
    }

    void UKPlugin::ApplyFunctionToAllFlightplans(
        std::function<void(const EuroScopeCFlightPlanInterface&, const EuroScopeCRadarTargetInterface&)> function) const
    {
        EuroScopePlugIn::CFlightPlan current = this->FlightPlanSelectFirst();

        // If there's nothing, stop
        if (!current.IsValid() || strcmp(current.GetCallsign(), "") == 0) {
            return;
        }

        // Loop through all visible flightplans
        do {
            if (!current.IsValid()) {
                continue;
            }

            EuroScopePlugIn::CRadarTarget rt = this->RadarTargetSelect(current.GetCallsign());

            if (!rt.IsValid()) {
                continue;
            }

            function(EuroScopeCFlightPlanWrapper(current), EuroScopeCRadarTargetWrapper(rt));

        } while (strcmp((current = this->FlightPlanSelectNext(current)).GetCallsign(), "") != 0);
    }

    void UKPlugin::ShowTextEditPopup(RECT editArea, int callbackId, std::string initialValue)
    {
        OpenPopupEdit(editArea, callbackId, initialValue.c_str());
    }

    /*
        Called when a handoff is initiated between controllers
    */
    void UKPlugin::OnFlightPlanFlightStripPushed(
        EuroScopePlugIn::CFlightPlan flightplan, const char* sendingController, const char* targetController)
    {
        EuroScopePlugIn::CController sender = std::string(sendingController) == this->ControllerMyself().GetCallsign()
                                                  ? this->ControllerMyself()
                                                  : this->ControllerSelect(sendingController);

        EuroScopePlugIn::CController target = std::string(targetController) == this->ControllerMyself().GetCallsign()
                                                  ? this->ControllerMyself()
                                                  : this->ControllerSelect(targetController);

        if (!sender.IsValid() || !target.IsValid()) {
            return;
        }

        EuroScopeCFlightPlanWrapper flightplanWrapper(flightplan);
        EuroScopeCControllerWrapper senderWrapper(sender, this->ControllerIsMe(sender, this->ControllerMyself()));
        EuroScopeCControllerWrapper targetWrapper(target, this->ControllerIsMe(target, this->ControllerMyself()));

        this->controllerHandoffHandlers.HandoffInitiated(flightplanWrapper, senderWrapper, targetWrapper);
    }

    /*
     * Set the ASEL flightplan
     */
    void UKPlugin::SetEuroscopeSelectedFlightplan(std::shared_ptr<EuroScopeCFlightPlanInterface> flightplan)
    {
        if (flightplan == nullptr) {
            return;
        }

        this->SetEuroscopeSelectedFlightplan(*flightplan);
    }

    void UKPlugin::SetEuroscopeSelectedFlightplan(const EuroScopeCFlightPlanInterface& flightplan)
    {
        this->SetASELAircraft(flightplan.GetEuroScopeObject());
    }

    /*
        Called on a timer by EuroScope.
    */
    void UKPlugin::OnTimer(int time)
    {
        this->timedEvents.Tick(time);
    }
} // namespace UKControllerPlugin
