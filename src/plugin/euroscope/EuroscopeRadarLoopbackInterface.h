#pragma once
#include "plugin/PopupMenuItem.h"

namespace UKControllerPlugin::Euroscope {

    /*
        Interface that allows components to interact directly with a Euroscope
        radar screen in a controlled manner. Also allows for testing,
    */
    class EuroscopeRadarLoopbackInterface
    {
        public:
        EuroscopeRadarLoopbackInterface();
        virtual ~EuroscopeRadarLoopbackInterface();
        EuroscopeRadarLoopbackInterface(const EuroscopeRadarLoopbackInterface&);
        EuroscopeRadarLoopbackInterface(EuroscopeRadarLoopbackInterface&&) = delete;
        [[nodiscard]] auto operator=(const EuroscopeRadarLoopbackInterface&) -> EuroscopeRadarLoopbackInterface&;
        [[nodiscard]] auto operator=(EuroscopeRadarLoopbackInterface&&) -> EuroscopeRadarLoopbackInterface& = delete;
        virtual void AddMenuItem(UKControllerPlugin::Plugin::PopupMenuItem menuItem) = 0;
        [[nodiscard]] virtual auto GetRadarViewport() -> RECT = 0;
        virtual void RegisterScreenObject(int objectType, std::string objectId, RECT location, bool moveable) = 0;
        [[nodiscard]] virtual auto GetAsrData(std::string key) -> std::string = 0;
        [[nodiscard]] virtual auto GetGroundspeedForCallsign(std::string cs) -> int = 0;
        [[nodiscard]] virtual auto PositionOffScreen(EuroScopePlugIn::CPosition pos) -> bool = 0;
        virtual void ToggleEuroscopeTagFunction(
            int functionId, const std::string& callsign, const POINT& mousePos, const RECT& tagItemArea) = 0;
        virtual void ToggleTemporaryAltitudePopupList(std::string callsign, POINT mousePos, RECT tagItemArea) = 0;
        virtual void
        TogglePluginTagFunction(std::string callsign, int functionId, POINT mousePos, RECT tagItemArea) = 0;
        virtual void ToogleMenu(RECT area, std::string title, int numColumns) = 0;
        [[nodiscard]] virtual auto ConvertCoordinateToScreenPoint(EuroScopePlugIn::CPosition pos) -> POINT = 0;
        [[nodiscard]] virtual auto ConvertScreenPointToCoordinate(const POINT& point) -> EuroScopePlugIn::CPosition = 0;
    };
} // namespace UKControllerPlugin::Euroscope
