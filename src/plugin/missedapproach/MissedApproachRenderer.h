#pragma once
#include "MissedApproachRenderMode.h"
#include "ownership/ServiceType.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership
} // namespace UKControllerPlugin

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;
    class MissedApproachRenderOptions;
    class MissedApproachOptions;

    /**
     * Renders recent missed approaches to the screen
     * so that users can see aircraft conducting missed
     * approaches.
     */
    class MissedApproachRenderer : public RadarScreen::RadarRenderableInterface
    {
        public:
        MissedApproachRenderer(
            std::shared_ptr<MissedApproachCollection> missedApproaches,
            const Ownership::AirfieldServiceProviderCollection& serviceProviders,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            std::shared_ptr<const MissedApproachRenderOptions> renderOptions,
            std::shared_ptr<const MissedApproachOptions> options);
        [[nodiscard]] auto IsVisible() const -> bool override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;

        private:
        [[nodiscard]] static auto GetLineEndpointCoordinates(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            const Gdiplus::Point& screenCentre,
            const EuroScopePlugIn::CPosition& aircraftPosition,
            const Gdiplus::Rect& aircraftBoundingRect) -> Gdiplus::Point;

        [[nodiscard]] auto CreatePen() const -> std::shared_ptr<Gdiplus::Pen>;

        // Contains all the missed approaches currently active
        const std::shared_ptr<MissedApproachCollection> missedApproaches;

        // Who's controlling what and where
        const Ownership::AirfieldServiceProviderCollection& serviceProviders;

        // Lets us access flightplans and radar targets
        const Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The options for this renderer
        const std::shared_ptr<const MissedApproachRenderOptions> renderOptions;

        // Global settings
        const std::shared_ptr<const MissedApproachOptions> options;

        // Render things
        static const INT CIRCLE_RENDER_SIZE_PX = 40;

        // The pen to draw with
        const Gdiplus::REAL PEN_WIDTH = 3;
        const Gdiplus::Color DRAW_COLOUR = Gdiplus::Color(255, 0, 0);
        const std::shared_ptr<Gdiplus::Pen> DRAW_PEN;
    };
} // namespace UKControllerPlugin::MissedApproach
