#pragma once
#include "MissedApproachRenderMode.h"
#include "euroscope/AsrEventHandlerInterface.h"
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

    /**
     * Renders recent missed approaches to the screen
     * so that users can see aircraft conducting missed
     * approaches.
     */
    class MissedApproachRenderer : public RadarScreen::RadarRenderableInterface,
                                   public Euroscope::AsrEventHandlerInterface
    {
        public:
        MissedApproachRenderer(
            std::shared_ptr<MissedApproachCollection> missedApproaches,
            const Ownership::AirfieldServiceProviderCollection& serviceProviders,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;
        [[nodiscard]] auto IsVisible() const -> bool override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;

        private:
        [[nodiscard]] static auto GetLineEndpointCoordinates(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            const Gdiplus::Point& screenCentre,
            const EuroScopePlugIn::CPosition aircraftPosition,
            const Gdiplus::Rect& aircraftBoundingRect
        ) -> Gdiplus::Point;
        
        [[nodiscard]] auto CreatePen() const -> std::shared_ptr<Gdiplus::Pen>;
        inline static const unsigned int DEFAULT_RENDER_DURATION = 55;

        // Contains all the missed approaches currently active
        const std::shared_ptr<MissedApproachCollection> missedApproaches;
        
        // Who's controlling what and where
        const Ownership::AirfieldServiceProviderCollection& serviceProviders;

        // Lets us access flightplans and radar targets
        const Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The render mode for missed approaches
        MissedApproachRenderMode renderMode;

        // We should render when providing which services
        Ownership::ServiceType renderForServiceProvision;

        // How long to render missed approaches for
        std::chrono::seconds renderDuration;
        
        // Which airfields we care about
        std::vector<std::string> renderFor;

        const std::string RENDER_MODE_ASR_KEY = "missedApproachRenderMode";
        const std::string RENDER_MODE_ASR_DESC = "Missed Approach Render Mode";
        const std::string RENDER_FOR_ASR_KEY = "missedApproachRenderFor";
        const std::string RENDER_FOR_ASR_DESC = "Missed Approach Render For";
        const std::string RENDER_DURATION_ASR_KEY = "missedApproachRenderDuration";
        const std::string RENDER_DURATION_ASR_DESC = "Missed Approach Render Duration";
        const std::string SERVICE_PROVISION_ASR_KEY = "missedApproachRenderForServiceProvision";
        const std::string SERVICE_PROVISION_ASR_DESC = "Missed Approach Render For Service Provision";
        
        // Render things
        static const INT CIRCLE_RENDER_SIZE_PX = 40;
    
        // The pen to draw with
        const Gdiplus::REAL PEN_WIDTH = 3;
        const Gdiplus::Color DRAW_COLOUR = Gdiplus::Color(255, 0, 0);
        const std::shared_ptr<Gdiplus::Pen> DRAW_PEN;
    };
} // namespace UKControllerPlugin::MissedApproach
