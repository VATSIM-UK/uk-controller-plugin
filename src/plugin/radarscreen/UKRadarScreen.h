#pragma once
#include "RadarRenderableCollection.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSettingProviderInterface.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class PluginSettingsProviderCollection;
        class UserSetting;
    } // namespace Euroscope
    namespace RadarScreen {
        class RadarRenderableCollection;
    } // namespace RadarScreen
    namespace Windows {
        class GdiGraphicsInterface;
    } // namespace Windows
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    /*
        Class for displaying to the default EuroScope Radar Screen.

        This class handles the rendering of the various modules and screen objects.
        It also contains the logic for what to do when objects on the screen are
        clicked or moved - which involves saving data to the loaded ASR file. In the vast majority of
        cases, this is delegated to other classes.
    */
    class UKRadarScreen : public EuroScopePlugIn::CRadarScreen,
                          public UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface,
                          public UKControllerPlugin::Euroscope::UserSettingProviderInterface
    {
        public:
        UKRadarScreen(
            UKControllerPlugin::Euroscope::AsrEventHandlerCollection userSettingEventHandler,
            const UKControllerPlugin::RadarScreen::RadarRenderableCollection& renderers,
            UKControllerPlugin::Command::CommandHandlerCollection commandHandlers,
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            const Euroscope::PluginSettingsProviderCollection& pluginSettingsProviders);
        ~UKRadarScreen() override;
        UKRadarScreen(const UKRadarScreen&) = delete;
        UKRadarScreen(UKRadarScreen&&) noexcept = delete;
        auto operator=(const UKRadarScreen&) -> UKRadarScreen& = delete;
        auto operator=(UKRadarScreen&&) -> UKRadarScreen& = delete;
        void AddMenuItem(UKControllerPlugin::Plugin::PopupMenuItem menuItem) override;
        auto ConvertCoordinateToScreenPoint(EuroScopePlugIn::CPosition pos) -> POINT override;
        [[nodiscard]] auto ConvertScreenPointToCoordinate(const POINT& point) -> EuroScopePlugIn::CPosition override;
        auto GetAsrData(std::string key) -> std::string override;
        auto GetGroundspeedForCallsign(std::string cs) -> int override;
        auto GetKey(std::string key) -> std::string override;
        auto GetRadarViewport() -> RECT override;
        auto KeyExists(std::string key) -> bool override;
        void OnAsrContentToBeClosed() override;
        void OnAsrContentToBeSaved() override;
        void OnAsrContentLoaded(bool loaded) override;
        void OnClickScreenObject(int ObjectType, const char* sObjectId, POINT Pt, RECT Area, int Button) override;
        auto OnCompileCommand(const char* sCommandLine) -> bool override;
        void OnMoveScreenObject(int ObjectType, const char* sObjectId, POINT Pt, RECT Area, bool Released) override;
        void OnRefresh(HDC hdc, int phase) override;
        auto PositionOffScreen(EuroScopePlugIn::CPosition pos) -> bool override;
        void RegisterScreenObject(int objectType, std::string objectId, RECT location, bool moveable) override;
        void SetKey(std::string key, std::string description, std::string value) override;
        void ToggleTemporaryAltitudePopupList(std::string callsign, POINT mousePos, RECT tagItemArea) override;
        void TogglePluginTagFunction(std::string callsign, int functionId, POINT mousePos, RECT tagItemArea) override;
        void ToogleMenu(RECT area, std::string title, int numColumns) override;

        private:
        // Wrapper around the ASR to provide access to user settings
        std::unique_ptr<UKControllerPlugin::Euroscope::UserSetting> userSettingProvider;

        // Wrapper around the Gdiplus Graphics library
        UKControllerPlugin::Windows::GdiGraphicsInterface& graphics;

        // Handles events relating to the ASR
        const UKControllerPlugin::Euroscope::AsrEventHandlerCollection userSettingEventHandler;

        // All the things that render things to the screen
        const UKControllerPlugin::RadarScreen::RadarRenderableCollection& renderers;

        // Processes Euroscope dot commands
        const UKControllerPlugin::Command::CommandHandlerCollection commandHandlers;

        // For handling plugin level settings. See Dtor explanation for why...
        const Euroscope::PluginSettingsProviderCollection& pluginSettingsProviders;

        // Has OnAsrContentLoaded been called?
        bool asrContentLoaded;

        // Last HDC context
        HDC lastContext;
    };
} // namespace UKControllerPlugin
