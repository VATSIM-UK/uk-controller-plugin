#pragma once
#include "command/CommandHandlerInterface.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace HistoryTrail {
        class HistoryTrailRepository;
    } // namespace HistoryTrail
    namespace Euroscope {
        class UserSetting;
        class EuroscopeRadarLoopbackInterface;
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope

    namespace Windows {
        class GdiGraphicsInterface;
        struct GdiplusBrushes;
    } // namespace Windows
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
} // namespace UKControllerPlugin

namespace Gdiplus {
    class Pen;
    class RectF;
} // namespace Gdiplus
// END

namespace UKControllerPlugin::HistoryTrail {

    /*
        A class for rendering history trails to the screen.
    */
    class HistoryTrailRenderer : public RadarScreen::ConfigurableDisplayInterface,
                                 public RadarScreen::RadarRenderableInterface,
                                 public Euroscope::AsrEventHandlerInterface,
                                 public Command::CommandHandlerInterface
    {
        public:
        HistoryTrailRenderer(
            const HistoryTrailRepository& trails,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const Dialog::DialogManager& dialogManager,
            int toggleCallbackFunctionId);
        void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        [[nodiscard]] auto GetAlphaPerDot() const -> int;
        [[nodiscard]] auto GetAntiAliasedTrails() const -> bool;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> Plugin::PopupMenuItem override;
        [[nodiscard]] auto GetDegradingTrails() const -> bool;
        [[nodiscard]] auto GetFadingTrails() const -> bool;
        [[nodiscard]] auto GetHistoryTrailLength() const -> int;
        [[nodiscard]] auto GetHistoryTrailType() const -> int;
        [[nodiscard]] auto GetHistoryTrailDotSize() const -> int;
        [[nodiscard]] auto GetMaximumAltitudeFilter() const -> int;
        [[nodiscard]] auto GetMinimumAltitudeFilter() const -> int;
        [[nodiscard]] auto GetFilledDots() const -> bool;
        [[nodiscard]] auto GetRotatedDots() const -> bool;
        [[nodiscard]] auto GetTrailColour() const -> Gdiplus::Color&;
        [[nodiscard]] auto IsVisible() const -> bool override;
        void Move(RECT position, std::string objectDescription) override;
        auto ProcessCommand(std::string command) -> bool override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override;

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

        // Dot filler settings
        const std::string dotFillUserSettingKey = "HistoryTrailDotFill";
        const std::string dotFillUserSettingDescription = "UKCP History Trail Filled Dots";

        // Dot rotation settings
        const std::string dotRotateUserSettingKey = "HistoryTrailDotTotate";
        const std::string dotRotateUserSettingDescription = "UKCP History Trail Rotated Dots";

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

        // Line trail type
        const int trailTypeLine = 3;

        // Default minimum altitude for altitude filter
        const int defaultMinAltitude = 0;

        // Default max altitude for altitude filter
        const int defaultMaxAltitude = 99999;

        private:
        [[nodiscard]] auto GetDoDotFunction() const
            -> std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::RectF&)>;
        [[nodiscard]] auto GetFillDotFunction() const
            -> std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::RectF&)>;
        [[nodiscard]] auto GetDrawDotFunction() const
            -> std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::RectF&)>;

        // Handles dialogs
        const Dialog::DialogManager& dialogManager;

        // The plugin, so we can check altitudes
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The history trail repository
        const HistoryTrailRepository& trails;

        // The colour to draw the trails with (or just the first colour, if fading)
        std::unique_ptr<Gdiplus::Color> startColour;

        // The pen with which to draw the trails
        std::unique_ptr<Gdiplus::Pen> pen;

        // The brush with which to draw the trails
        std::unique_ptr<Gdiplus::SolidBrush> brush;

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

        // Should the dots be filled
        bool filledDots;

        // Should the dots be rotated
        bool rotatedDots;

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

        // Function for drawing the dot - saved as a lambda so we dont have to keep checking the trail type
        std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::RectF&)> drawDot;
    };
} // namespace UKControllerPlugin::HistoryTrail
