#pragma once
#include "plugin/PopupMenuItem.h"
#include "euroscope/EuroScopePlugIn.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Interface that allows components to interact directly with a Euroscope
            radar screen in a controlled manner. Also allows for testing,
        */
        class EuroscopeRadarLoopbackInterface
        {
            public:
                virtual void AddMenuItem(UKControllerPlugin::Plugin::PopupMenuItem menuItem) = 0;
                virtual RECT GetRadarViewport(void) = 0;
                virtual void RegisterScreenObject(
                    int objectType,
                    std::string objectId,
                    RECT location,
                    bool moveable
                ) = 0;
                virtual std::string GetAsrData(std::string key) = 0;
                virtual int GetGroundspeedForCallsign(std::string cs) = 0;
                virtual bool HasAsrKey(std::string key) = 0;
                virtual bool PositionOffScreen(EuroScopePlugIn::CPosition pos) = 0;
                virtual void ToggleTemporaryAltitudePopupList(
                    std::string callsign,
                    POINT mousePos,
                    RECT tagItemArea
                ) = 0;
                virtual void TogglePluginTagFunction(
                    std::string callsign,
                    int functionId,
                    POINT mousePos,
                    RECT tagItemArea
                ) = 0;
                virtual void ToogleMenu(RECT area, std::string title, int numColumns) = 0;
                virtual POINT ConvertCoordinateToScreenPoint(EuroScopePlugIn::CPosition pos) = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
