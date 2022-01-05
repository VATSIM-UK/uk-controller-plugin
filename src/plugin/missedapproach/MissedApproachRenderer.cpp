#include "MissedApproach.h"
#include "MissedApproachCollection.h"
#include "MissedApproachOptions.h"
#include "MissedApproachRenderer.h"
#include "MissedApproachRenderOptions.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "headings/Heading.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Headings::Heading;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPlugin::MissedApproach {

    MissedApproachRenderer::MissedApproachRenderer(
        std::shared_ptr<MissedApproachCollection> missedApproaches,
        const Ownership::AirfieldServiceProviderCollection& serviceProviders,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        std::shared_ptr<const MissedApproachRenderOptions> renderOptions,
        std::shared_ptr<const MissedApproachOptions> options)
        : missedApproaches(std::move(missedApproaches)), serviceProviders(serviceProviders), plugin(plugin),
          renderOptions(std::move(renderOptions)), options(std::move(options)), DRAW_PEN(CreatePen())
    {
    }

    auto MissedApproachRenderer::IsVisible() const -> bool
    {
        return this->renderOptions->Mode() != MissedApproachRenderMode::None;
    }

    void MissedApproachRenderer::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        const auto airfieldsProvidingServices =
            this->serviceProviders.GetAirfieldsWhereUserProvidingServices(this->options->ServiceProvisions());
        const auto& alwaysRenderFor = this->options->Airfields();

        if (airfieldsProvidingServices.empty() && alwaysRenderFor.empty()) {
            return;
        }

        // Combine the "always alerts" and the "service provision" alerts to get all relevant airfields
        std::vector<std::string> relevantAirfields;
        relevantAirfields.reserve(airfieldsProvidingServices.size() + alwaysRenderFor.size());
        relevantAirfields.insert(
            relevantAirfields.end(), airfieldsProvidingServices.begin(), airfieldsProvidingServices.end());
        relevantAirfields.insert(relevantAirfields.end(), alwaysRenderFor.begin(), alwaysRenderFor.end());

        this->missedApproaches->ForEach([this, &relevantAirfields, &radarScreen, &graphics](
                                            const std::shared_ptr<class MissedApproach>& missed) {
            // Only display for certain duration
            if (TimeNow() > missed->CreatedAt() + this->renderOptions->Duration()) {
                return;
            }

            // Only display if relevant to us
            const auto flightplan = this->plugin.GetFlightplanForCallsign(missed->Callsign());
            const auto radarTarget = this->plugin.GetRadarTargetForCallsign(missed->Callsign());

            if (!flightplan || !radarTarget ||
                std::find(relevantAirfields.begin(), relevantAirfields.end(), flightplan->GetDestination()) ==
                    relevantAirfields.cend()) {
                return;
            }

            // Draw the line if we need to
            const auto radarTargetPosition = radarTarget->GetPosition();
            const auto screenCoordinate = radarScreen.ConvertCoordinateToScreenPoint(radarTargetPosition);
            Gdiplus::Rect boundingRect{
                screenCoordinate.x - (CIRCLE_RENDER_SIZE_PX / 2),
                screenCoordinate.y - (CIRCLE_RENDER_SIZE_PX / 2),
                CIRCLE_RENDER_SIZE_PX,
                CIRCLE_RENDER_SIZE_PX};
            if ((this->renderOptions->Mode() & MissedApproachRenderMode::Line) == MissedApproachRenderMode::Line) {
                const auto viewport = radarScreen.GetRadarViewport();
                const Gdiplus::Point radarCentre{
                    viewport.left + ((viewport.right - viewport.left) / 2),
                    viewport.top + ((viewport.bottom - viewport.top) / 2),
                };

                graphics.DrawLine(
                    *this->DRAW_PEN,
                    radarCentre,
                    GetLineEndpointCoordinates(radarScreen, radarCentre, radarTargetPosition, boundingRect));
            }

            // Draw the circle if we need to
            if ((this->renderOptions->Mode() & MissedApproachRenderMode::Circle) == MissedApproachRenderMode::Circle) {
                graphics.DrawCircle(boundingRect, *this->DRAW_PEN);
            }
        });
    }

    auto MissedApproachRenderer::CreatePen() const -> std::shared_ptr<Gdiplus::Pen>
    {
        auto pen = std::make_shared<Gdiplus::Pen>(DRAW_COLOUR, PEN_WIDTH);
        pen->SetDashStyle(Gdiplus::DashStyleDashDotDot);
        return pen;
    }

    auto MissedApproachRenderer::GetLineEndpointCoordinates(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        const Gdiplus::Point& screenCentre,
        const EuroScopePlugIn::CPosition& aircraftPosition,
        const Gdiplus::Rect& aircraftBoundingRect) -> Gdiplus::Point
    {
        const auto screenCentreCoordinate =
            radarScreen.ConvertScreenPointToCoordinate({screenCentre.X, screenCentre.Y});
        const auto headingFromAircraftToScreenCentre = aircraftPosition.DirectionTo(screenCentreCoordinate);

        // Use the middle of the respective bounding line as the end
        if (headingFromAircraftToScreenCentre >= Heading::NE && headingFromAircraftToScreenCentre < Heading::SE) {
            return {aircraftBoundingRect.GetRight(), aircraftBoundingRect.GetTop() + (aircraftBoundingRect.Height / 2)};
        }

        if (headingFromAircraftToScreenCentre >= Heading::SE && headingFromAircraftToScreenCentre < Heading::SW) {
            return {
                aircraftBoundingRect.GetLeft() + (aircraftBoundingRect.Width / 2), aircraftBoundingRect.GetBottom()};
        }

        if (headingFromAircraftToScreenCentre >= Heading::SW && headingFromAircraftToScreenCentre < Heading::NW) {
            return {aircraftBoundingRect.GetLeft(), aircraftBoundingRect.GetTop() + (aircraftBoundingRect.Height / 2)};
        }

        return {aircraftBoundingRect.GetLeft() + (aircraftBoundingRect.Width / 2), aircraftBoundingRect.GetTop()};
    }
} // namespace UKControllerPlugin::MissedApproach
