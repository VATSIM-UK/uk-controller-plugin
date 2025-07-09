#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
        class ControllerPositionCollection;
    } // namespace Controller
    namespace Components {
        class BrushSwitcher;
        class TitleBar;
        class Button;
    } // namespace Components
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Prenote {
        class PrenoteMessage;
        class PrenoteMessageCollection;
    } // namespace Prenote
    namespace Releases {
        class DepartureReleaseRequest;
        class DepartureReleaseEventHandler;
    } // namespace Releases
    namespace Windows {
        class GdiGraphicsInterface;
        struct GdiplusBrushes;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Departure {

    /*
     * Renders a list of departure releases and prenotes requiring the users
     * attention.
     */
    class DepartureCoordinationList : public RadarScreen::RadarRenderableInterface,
                                      public Euroscope::AsrEventHandlerInterface
    {
        public:
        DepartureCoordinationList(
            Releases::DepartureReleaseEventHandler& handler,
            Prenote::PrenoteMessageCollection& prenotes,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const Controller::ControllerPositionCollection& controllers,
            const Controller::ActiveCallsignCollection& activeCallsigns,
            const UKControllerPlugin::Windows::GdiplusBrushes& brushes,
            int screenObjectId);
        void LeftClick(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        auto IsVisible() const -> bool override;
        void Move(RECT position, std::string objectDescription) override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override;
        void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;
        void ToggleVisible();
        auto ContentCollapsed() const -> bool;
        auto Position() const -> Gdiplus::PointF;

        private:
        static auto GetAsrKey(const std::string& item) -> std::string;
        static auto GetAsrDescription(const std::string& description) -> std::string;

        // The controllers
        const Controller::ControllerPositionCollection& controllers;

        // Handles events for departure releases
        Releases::DepartureReleaseEventHandler& handler;

        // Contains all the prenote messages
        Prenote::PrenoteMessageCollection& prenotes;

        // Pens and brushes
        const UKControllerPlugin::Windows::GdiplusBrushes& brushes;

        // Provides interface with the plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Who's actively online
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // Drawing RECTs
        const Gdiplus::Rect typeColumnHeader{5, 5, 40, 25};
        const Gdiplus::Rect callsignColumnHeader{50, 5, 100, 25};
        const Gdiplus::Rect controllerColumnHeader{160, 5, 100, 25};
        const Gdiplus::Rect airportColumnHeader{270, 5, 40, 25};
        const Gdiplus::Rect sidColumnHeader{320, 5, 65, 25};
        const Gdiplus::Rect destColumnHeader{395, 5, 40, 25};

        // Clickspot identifier
        const int screenObjectId;

        // Is the window visible
        bool visible;

        // Is the content collapsed
        bool contentCollapsed;

        // Height of title bar
        const int titleBarHeight = 15;

        // Width of title bar
        const int titleBarWidth = 435;

        // Default X/Y of window
        static const int defaultPosition = 100;

        // Default RECT on reset position
        inline static const RECT defaultRect = {100, 100, 200, 200};
        const Gdiplus::Rect closeButtonOffset = {415, 5, 10, 10};
        const Gdiplus::Rect collapseButtonOffset = {400, 5, 10, 10};

        // How high each line is
        static const int lineHeight = 25;

        // Our window position
        Gdiplus::PointF position = {0, 0};

        // Standard window components
        std::shared_ptr<Components::TitleBar> titleBar;
        std::shared_ptr<Components::Button> closeButton;
        std::shared_ptr<Components::Button> collapseButton;
        std::shared_ptr<Components::BrushSwitcher> brushSwitcher;

        // Some TAG function ids
        inline static const int DEPARTURE_RELEASE_DECISION_TAG_FUNCTION_ID = 9013;
        inline static const int ACKNOWLEDGE_PRENOTE_TAG_FUNCTION_ID = 9019;
    };
} // namespace UKControllerPlugin::Departure
