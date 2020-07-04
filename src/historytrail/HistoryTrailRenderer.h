#pragma once
#include "radarscreen/RadarRenderableInterface.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "command/CommandHandlerInterface.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace HistoryTrail {
        class HistoryTrailRepository;
    }  // namespace HistoryTrail
    namespace Euroscope {
        class UserSetting;
        class EuroscopeRadarLoopbackInterface;
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope

    namespace Windows {
        class GdiGraphicsInterface;
        struct GdiplusBrushes;
    }  // namespace Windows
    namespace Dialog {
        class DialogManager;
    }  // namespace Dialog
}  // namespace UKControllerPlugin

namespace Gdiplus {
    class Pen;
    class RectF;
}  // namespace Gdiplus
// END

namespace UKControllerPlugin {
    namespace HistoryTrail {

        /*
            A class for rendering history trails to the screen.
        */
        class HistoryTrailRenderer :
            public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
            public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
            public UKControllerPlugin::Euroscope::AsrEventHandlerInterface,
            public UKControllerPlugin::Command::CommandHandlerInterface
        {
            public:
                HistoryTrailRenderer(
                    const UKControllerPlugin::HistoryTrail::HistoryTrailRepository & trails,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    const UKControllerPlugin::Dialog::DialogManager & dialogManager,
                    int toggleCallbackFunctionId
                );
                void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;
                void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                int GetAlphaPerDot(void) const;
                bool GetAntiAliasedTrails(void) const;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;
                bool GetDegradingTrails(void) const;
                bool GetFadingTrails(void) const;
                int GetHistoryTrailLength(void) const;
                int GetHistoryTrailType(void) const;
                int GetHistoryTrailDotSize(void) const;
                int GetMaximumAltitudeFilter(void) const;
                int GetMinimumAltitudeFilter(void) const;
                Gdiplus::Color & GetTrailColour(void) const;
                bool IsVisible(void) const override;
                void LeftClick(
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
                    int objectId,
                    std::string objectDescription,
                    POINT mousePos,
                    RECT itemArea
                ) override;
                void Move(RECT position, std::string objectDescription) override;
                bool ProcessCommand(std::string command) override;
                void RightClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void Render(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void ResetPosition(void) override;

                // The function ID for the toggle callback
                const int toggleCallbackFunctionId;

                // The resource id for the HistoryTrail dialog;
                const int configurationDialogId = IDD_HISTORY_TRAIL;

                // Visibility ASR settings
                const std::string visibleUserSettingKey = "HistoryTrailDisplay";

                const std::string visibleUserSettingDescription = "Display UKCP History Trails";

                // Trail type ASR settings
                const std::string trailTypeUserSettingKey = "HistoryTrailType";

                const std::string trailTypeUserSettingDescription = "UKCP History Trail Type";

                // Dot size ASR settings
                const std::string dotSizeUserSettingKey = "HistoryTrailDotSize";

                const std::string dotSizeUserSettingDescription = "UKCP History Trail Dot Size";

                // Degrading ASR settings
                const std::string degradingUserSettingKey = "HistoryTrailDegrade";

                const std::string degradingUserSettingDescription = "UKCP History Trail Degrade";

                // Fading ASR settings
                const std::string fadingUserSettingKey = "HistoryTrailFade";

                const std::string fadingUserSettingDescription = "UKCP History Trail Fade";

                // AA ASR settings
                const std::string antialiasUserSettingKey = "HistoryTrailAntiAlias";

                const std::string antialiasUserSettingDescription = "UKCP History Trail Antialias";

                // Trail length ASR settings
                const std::string trailLengthUserSettingKey = "HistoryTrailLength";

                const std::string trailLengthUserSettingDescription = "UKCP History Trail Length";

                // Trail colour ASR settings
                const std::string trailColourUserSettingKey = "HistoryTrailColour";

                const std::string trailColourUserSettingDescription = "UKCP History Trail Colour";

                // Min altitude filter settings
                const std::string minAltitudeFilterUserSettingKey = "HistoryTrailMinAltitudeFilter";
                const std::string minAltitudeFilterUserSettingDescription = "UKCP Minimum History Trail Altitude";

                // Max altitude filter settings
                const std::string maxAltitudeFilterUserSettingKey = "HistoryTrailMaxAltitudeFilter";
                const std::string maxAltitudeFilterUserSettingDescription = "UKCP Maximum History Trail Altitude";

                // The module menu text
                const std::string menuItemDescription = "Configure History Trails";

                // The minimum groundspeed to display history trails for.
                const int minimumSpeed = 50;

                // Diamond trail type
                const int trailTypeDiamond = 0;

                // Square trail type
                const int trailTypeSquare = 1;

                // Circle trail type
                const int trailTypeCircle = 2;

                // Default minimum altitude for altitude filter
                const int defaultMinAltitude = 0;

                // Default max altitude for altitude filter
                const int defaultMaxAltitude = 99999;


            private:

                void DrawDot(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    Gdiplus::Pen & pen,
                    const Gdiplus::RectF & area
                );

                // Handles dialogs
                const UKControllerPlugin::Dialog::DialogManager & dialogManager;

                // The plugin, so we can check altitudes
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // The history trail repository
                const UKControllerPlugin::HistoryTrail::HistoryTrailRepository & trails;

                // The colour to draw the trails with (or just the first colour, if fading)
                std::unique_ptr<Gdiplus::Color> startColour;

                // The pen with which to draw the trails
                std::unique_ptr<Gdiplus::Pen> pen;

                // Whether or not we should render the trails.
                bool visible;

                // The type of history trail.
                int historyTrailType;

                // History trail dotsize as a float
                float historyTrailDotSizeFloat;

                // The size of each history trail dot
                int historyTrailDotSize;

                // Whether trails should degrade over time
                bool degradingTrails;

                // Whether or not trails should fade over time
                bool fadingTrails;

                // Whether or not trails should use antialiasing
                bool antialiasedTrails;

                // The length of trail to render
                int historyTrailLength;

                // The amount of alpha to reduce per dot
                int alphaPerDot;

                // The altitude at and below which not to display
                int minimumDisplayAltitude;

                // The altitude at and above which not to display
                int maximumDisplayAltitude;

                // The dot command for opening the configuration modal.
                const std::string dotCommand = ".ukcp h";
        };
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
