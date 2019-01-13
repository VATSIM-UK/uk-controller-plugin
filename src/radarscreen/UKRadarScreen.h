#pragma once
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSettingProviderInterface.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "command/CommandHandlerCollection.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace RadarScreen {
        class RadarRenderableCollection;
    }  // namespace RadarScreen
    namespace Windows {
        class GdiGraphicsInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin
// END


namespace UKControllerPlugin {

    // Forward declarations
    class HelperFunctions;
    // END

    /*
        Class for displaying to the default EuroScope Radar Screen.

        This class handles the rendering of the various modules and screen objects.
        It also contains the logic for what to do when objects on the screen are
        clicked or moved - which involves saving data to the loaded ASR file. In the vast majority of
        cases, this is delegated to other classes.
    */
    class UKRadarScreen :
        public EuroScopePlugIn::CRadarScreen, public UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface,
        public UKControllerPlugin::Euroscope::UserSettingProviderInterface
    {
        public:
            UKRadarScreen(
                const UKControllerPlugin::Euroscope::AsrEventHandlerCollection userSettingEventHandler,
                const UKControllerPlugin::RadarScreen::RadarRenderableCollection renderers,
                const UKControllerPlugin::Command::CommandHandlerCollection commandHandlers,
                UKControllerPlugin::Windows::GdiGraphicsInterface & graphics
            );
            ~UKRadarScreen(void);
            void AddMenuItem(UKControllerPlugin::Plugin::PopupMenuItem menuItem) override;
            POINT ConvertCoordinateToScreenPoint(EuroScopePlugIn::CPosition pos) override;
            std::string GetAsrData(std::string key) override;
            int GetGroundspeedForCallsign(std::string cs) override;
            std::string GetKey(std::string key) override;
            RECT GetRadarViewport(void);
            bool HasAsrKey(std::string key) override;
            bool KeyExists(std::string key) override;
            void OnAsrContentToBeClosed(void);
            void OnAsrContentToBeSaved(void);
            void OnAsrContentLoaded(bool loaded);
            void OnClickScreenObject(int ObjectType,
                const char * sObjectId,
                POINT Pt,
                RECT Area,
                int Button
            );
            bool OnCompileCommand(const char * sCommandLine);
            void OnMoveScreenObject(
                int ObjectType,
                const char * sObjectId,
                POINT Pt,
                RECT Area,
                bool Released
            );
            void OnRefresh(HDC hdc, int phase);
            bool PositionOffScreen(EuroScopePlugIn::CPosition pos) override;
            void RegisterScreenObject(int objectType, std::string objectId, RECT location, bool moveable) override;
            void SetKey(std::string key, std::string description, std::string value) override;
            void ToogleMenu(RECT area, std::string title, int numColumns) override;

        private:

            // Wrapper around the Gdiplus Graphics library
            UKControllerPlugin::Windows::GdiGraphicsInterface & graphics;

            // Handles events relating to the ASR
            const UKControllerPlugin::Euroscope::AsrEventHandlerCollection userSettingEventHandler;

            // All the things that render things to the screen
            const UKControllerPlugin::RadarScreen::RadarRenderableCollection renderers;

            // Processes Euroscope dot commands
            const UKControllerPlugin::Command::CommandHandlerCollection commandHandlers;

            // Has OnAsrContentLoaded been called?
            bool asrContentLoaded;

            // Last HDC context
            HDC lastContext;
    };
}  // namespace UKControllerPlugin
