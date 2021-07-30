#include "pch/stdafx.h"
#include "radarscreen/UKRadarScreen.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "helper/HelperFunctions.h"
#include "graphics/GdiGraphicsInterface.h"
#include "euroscope/UserSetting.h"
#include "update/PluginVersion.h"

using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PluginVersion;

namespace UKControllerPlugin {

    UKRadarScreen::UKRadarScreen(
        const UKControllerPlugin::Euroscope::AsrEventHandlerCollection userSettingEventHandler,
        const UKControllerPlugin::RadarScreen::RadarRenderableCollection & renderers,
        const UKControllerPlugin::Command::CommandHandlerCollection commandHandlers,
        UKControllerPlugin::Windows::GdiGraphicsInterface & graphics
    )
        : userSettingEventHandler(userSettingEventHandler), renderers(renderers),
        commandHandlers(commandHandlers), graphics(graphics)
    {

    }

    UKRadarScreen::~UKRadarScreen(void)
    {
        this->OnAsrContentToBeSaved();
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
            menuItem.fixedPosition
        );
    }

    /*
        Converts a coordinate from screen position to pixel.
    */
    POINT UKRadarScreen::ConvertCoordinateToScreenPoint(EuroScopePlugIn::CPosition pos)
    {
        return this->ConvertCoordFromPositionToPixel(pos);
    }

    /*
        Interface method, get data from the ASR.
    */
    std::string UKRadarScreen::GetAsrData(std::string key)
    {
        return (this->GetDataFromAsr(key.c_str()) != NULL) ? this->GetDataFromAsr(key.c_str()) : "";
    }

    /*
        Returns the groundspeed for a given callsign.
    */
    int UKRadarScreen::GetGroundspeedForCallsign(std::string cs)
    {
        return this->GetPlugIn()->RadarTargetSelect(cs.c_str()).GetGS();
    }

    /*
        Returns the viewpoint for the radar.
    */
    RECT UKRadarScreen::GetRadarViewport(void)
    {
        return this->GetRadarArea();
    }

    /*
        Returns true if an ASR key exists.
    */
    bool UKRadarScreen::HasAsrKey(std::string key)
    {
        return this->GetDataFromAsr(key.c_str()) != NULL;
    }

    /*
        Adds a screen object.
    */
    void UKRadarScreen::RegisterScreenObject(int objectType, std::string objectId, RECT location, bool moveable)
    {
        this->AddScreenObject(objectType, objectId.c_str(), location, moveable, NULL);
    }

    /*
        Returns true if a key exists in the ASR, false otherwise.
    */
    bool UKRadarScreen::KeyExists(std::string key)
    {
        return this->GetDataFromAsr(key.c_str()) != NULL;
    }

    /*
        Returns the value of a key in the ASR, empty string if it doesn't exist.
    */
    std::string UKRadarScreen::GetKey(std::string key)
    {
        return this->KeyExists(key) ? this->GetAsrData(key.c_str()) : "";
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
    void UKRadarScreen::OnAsrContentToBeClosed(void)
    {
        delete this;
    }

    void UKRadarScreen::OnAsrContentToBeSaved(void)
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
    void UKRadarScreen::OnClickScreenObject(int ObjectType,
        const char * sObjectId,
        POINT Pt,
        RECT Area,
        int Button
    ) {
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
    bool UKRadarScreen::OnCompileCommand(const char * sCommandLine)
    {
        return this->commandHandlers.ProcessCommand(sCommandLine);
    }

    /*
        Handles when an object on the screen is moved, e.g. title bars.
    */
    void UKRadarScreen::OnMoveScreenObject(
        int ObjectType,
        const char * sObjectId,
        POINT Pt,
        RECT Area,
        bool Released
    ) {
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

    bool UKRadarScreen::PositionOffScreen(EuroScopePlugIn::CPosition pos)
    {
        RECT radarArea = this->GetRadarArea();
        POINT posOnScreen = this->ConvertCoordinateToScreenPoint(pos);

        return posOnScreen.x > radarArea.right || posOnScreen.x < radarArea.left ||
            posOnScreen.y > radarArea.bottom || posOnScreen.y < radarArea.top;
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
            NULL,
            EuroScopePlugIn::TAG_ITEM_TYPE_CALLSIGN,
            callsign.c_str(),
            NULL,
            EuroScopePlugIn::TAG_ITEM_FUNCTION_TEMP_ALTITUDE_POPUP,
            mousePos,
            tagItemArea
        );
    }

    void UKRadarScreen::TogglePluginTagFunction(std::string callsign, int functionId, POINT mousePos, RECT tagItemArea)
    {
        this->GetPlugIn()->SetASELAircraft(this->GetPlugIn()->FlightPlanSelect(callsign.c_str()));
        this->StartTagFunction(
            callsign.c_str(),
            NULL,
            EuroScopePlugIn::TAG_ITEM_TYPE_CALLSIGN,
            callsign.c_str(),
            PluginVersion::title,
            functionId,
            mousePos,
            tagItemArea
        );
    }
}  // namespace UKControllerPlugin
