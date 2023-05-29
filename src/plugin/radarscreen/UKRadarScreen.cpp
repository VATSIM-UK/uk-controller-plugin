#include "RadarRenderableCollection.h"
#include "UKRadarScreen.h"
#include "euroscope/EuroScopeCFlightPlanWrapper.h"
#include "euroscope/EuroScopeCRadarTargetWrapper.h"
#include "euroscope/PluginSettingsProviderCollection.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "plugin/FunctionCallEventHandler.h"
#include "update/PluginVersion.h"

using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PluginVersion;

namespace UKControllerPlugin {

    UKRadarScreen::UKRadarScreen(
        UKControllerPlugin::Euroscope::AsrEventHandlerCollection userSettingEventHandler,
        const UKControllerPlugin::RadarScreen::RadarRenderableCollection& renderers,
        UKControllerPlugin::Command::CommandHandlerCollection commandHandlers,
        UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
        const Euroscope::PluginSettingsProviderCollection& pluginSettingsProviders,
        const Plugin::FunctionCallEventHandler& functionHandler)
        : graphics(graphics), userSettingEventHandler(std::move(userSettingEventHandler)), renderers(renderers),
          commandHandlers(std::move(commandHandlers)), pluginSettingsProviders(pluginSettingsProviders),
          functionHandler(functionHandler), asrContentLoaded(false), lastContext(nullptr)
    {
    }

    UKRadarScreen::~UKRadarScreen()
    {
        /**
         * So you may be wondering... why are we saving here? Good question.
         *
         * Unfortunately, whilst you can trigger saving ASR settings when the DTOR for a RadarScreen is called,
         * the same cannot be said for plugin-level settings when the plugin destructs.
         *
         * So, to workaround this (yay workaround!), we call the plugin-level save when the RadarScreen destructs, as
         * this is called early enough in the shutdown process to be able to quickly save settings.
         */
        pluginSettingsProviders.Save();

        this->OnAsrContentToBeSaved(); // NOLINT
    }

    /*
        Adds an item to the menu.
    */
    void UKRadarScreen::AddMenuItem(UKControllerPlugin::Plugin::PopupMenuItem menuItem)
    {
        this->GetPlugIn()->AddPopupListElement(
            menuItem.firstValue.c_str(),
            menuItem.secondValue.c_str(),
            menuItem.callbackFunctionId,
            false,
            menuItem.checked,
            menuItem.disabled,
            menuItem.fixedPosition);
    }

    /*
        Converts a coordinate from screen position to pixel.
    */
    auto UKRadarScreen::ConvertCoordinateToScreenPoint(EuroScopePlugIn::CPosition pos) -> POINT
    {
        return this->ConvertCoordFromPositionToPixel(pos);
    }

    /*
        Interface method, get data from the ASR.
    */
    auto UKRadarScreen::GetAsrData(std::string key) -> std::string
    {
        return (this->GetDataFromAsr(key.c_str()) != nullptr) ? this->GetDataFromAsr(key.c_str()) : "";
    }

    /*
        Returns the groundspeed for a given callsign.
    */
    auto UKRadarScreen::GetGroundspeedForCallsign(std::string cs) -> int
    {
        return this->GetPlugIn()->RadarTargetSelect(cs.c_str()).GetGS();
    }

    /*
        Returns the viewpoint for the radar.
    */
    auto UKRadarScreen::GetRadarViewport() -> RECT
    {
        return this->GetRadarArea();
    }

    /*
        Adds a screen object.
    */
    void UKRadarScreen::RegisterScreenObject(int objectType, std::string objectId, RECT location, bool moveable)
    {
        this->AddScreenObject(objectType, objectId.c_str(), location, moveable, nullptr);
    }

    /*
        Returns true if a key exists in the ASR, false otherwise.
    */
    auto UKRadarScreen::KeyExists(std::string key) -> bool
    {
        return this->GetDataFromAsr(key.c_str()) != nullptr;
    }

    /*
        Returns the value of a key in the ASR, empty string if it doesn't exist.
    */
    auto UKRadarScreen::GetKey(std::string key) -> std::string
    {
        return this->KeyExists(key) ? this->GetAsrData(key) : "";
    }

    /*
        Sets the value of a particular key in the ASR.
    */
    void UKRadarScreen::SetKey(std::string key, std::string description, std::string value)
    {
        this->SaveDataToAsr(key.c_str(), description.c_str(), value.c_str());
    }

    /*
        Has to be defined, because EuroScope. Have to call delete on current object
        in order to trigger last minute ASR content saving.
    */
    void UKRadarScreen::OnAsrContentToBeClosed()
    {
        delete this;
    }

    void UKRadarScreen::OnAsrContentToBeSaved()
    {
        this->userSettingEventHandler.AsrClosingEvent(*this->userSettingProvider);
    }

    /*
        Called by Euroscope for each UKRadarScreen instance once the data from
        the ASR file has been successfully loaded.
    */
    void UKRadarScreen::OnAsrContentLoaded(bool loaded)
    {
        this->userSettingProvider = std::make_unique<UserSetting>(*this);
        this->userSettingEventHandler.AsrLoadedEvent(*this->userSettingProvider);
        this->asrContentLoaded = true;
    }

    /*
        Handles when an object on the screen is clicked, e.g. buttons.
    */
    void UKRadarScreen::OnClickScreenObject(int ObjectType, const char* sObjectId, POINT Pt, RECT Area, int Button)
    {
        if (Button == EuroScopePlugIn::BUTTON_LEFT) {
            this->renderers.LeftClickScreenObject(*this, ObjectType, sObjectId, Pt, Area);
        } else if (Button == EuroScopePlugIn::BUTTON_RIGHT) {
            this->renderers.RightClickScreenObject(ObjectType, sObjectId, *this);
        }
    }

    /*
        Called when EuroScope takes a dot command.

        Should return true if we've accepted the command,
        false in all other cases.
    */
    auto UKRadarScreen::OnCompileCommand(const char* sCommandLine) -> bool
    {
        return this->commandHandlers.ProcessCommand(sCommandLine);
    }

    /*
        Handles when an object on the screen is moved, e.g. title bars.
    */
    void UKRadarScreen::OnMoveScreenObject(int ObjectType, const char* sObjectId, POINT Pt, RECT Area, bool Released)
    {
        this->renderers.MoveScreenObject(ObjectType, sObjectId, Area);
        this->RequestRefresh();
    }

    /*
        Called whenever a screen refresh happens. Only render the loaded modules.
    */
    void UKRadarScreen::OnRefresh(HDC hdc, int phase)
    {
        // Change the device handle in the graphics library only if it's been changed
        if (hdc != this->lastContext) {
            this->lastContext = hdc;
            this->graphics.SetDeviceHandle(hdc);
        }

        this->renderers.Render(phase, this->graphics, *this);
    }

    auto UKRadarScreen::PositionOffScreen(EuroScopePlugIn::CPosition pos) -> bool
    {
        RECT radarArea = this->GetRadarArea();
        POINT posOnScreen = this->ConvertCoordinateToScreenPoint(pos);

        return posOnScreen.x > radarArea.right || posOnScreen.x < radarArea.left || posOnScreen.y > radarArea.bottom ||
               posOnScreen.y < radarArea.top;
    }

    /*
        Toggles a menu.
    */
    void UKRadarScreen::ToogleMenu(RECT area, std::string title, int numColumns)
    {
        this->GetPlugIn()->OpenPopupList(area, title.c_str(), numColumns);
    }

    void UKRadarScreen::ToggleTemporaryAltitudePopupList(std::string callsign, POINT mousePos, RECT tagItemArea)
    {
        this->GetPlugIn()->SetASELAircraft(this->GetPlugIn()->FlightPlanSelect(callsign.c_str()));
        this->StartTagFunction(
            callsign.c_str(),
            nullptr,
            EuroScopePlugIn::TAG_ITEM_TYPE_CALLSIGN,
            callsign.c_str(),
            nullptr,
            EuroScopePlugIn::TAG_ITEM_FUNCTION_TEMP_ALTITUDE_POPUP,
            mousePos,
            tagItemArea);
    }

    void UKRadarScreen::TogglePluginTagFunction(std::string callsign, int functionId, POINT mousePos, RECT tagItemArea)
    {
        this->GetPlugIn()->SetASELAircraft(this->GetPlugIn()->FlightPlanSelect(callsign.c_str()));
        this->StartTagFunction(
            callsign.c_str(),
            nullptr,
            EuroScopePlugIn::TAG_ITEM_TYPE_CALLSIGN,
            callsign.c_str(),
            PluginVersion::title,
            functionId,
            mousePos,
            tagItemArea);
    }

    auto UKRadarScreen::ConvertScreenPointToCoordinate(const POINT& point) -> EuroScopePlugIn::CPosition
    {
        return this->ConvertCoordFromPixelToPosition(point);
    }

    void UKRadarScreen::OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area)
    {
        auto flightplan = Euroscope::EuroScopeCFlightPlanWrapper(this->GetPlugIn()->FlightPlanSelectASEL());
        auto radarTarget = Euroscope::EuroScopeCRadarTargetWrapper(this->GetPlugIn()->RadarTargetSelectASEL());
        this->functionHandler.CallFunction(*this, FunctionId, sItemString, flightplan, radarTarget, Pt, Area);
    }
} // namespace UKControllerPlugin
