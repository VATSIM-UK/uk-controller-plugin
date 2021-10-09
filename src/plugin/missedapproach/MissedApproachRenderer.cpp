#include "MissedApproach.h"
#include "MissedApproachCollection.h"
#include "MissedApproachRenderer.h"
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
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : missedApproaches(std::move(missedApproaches)), serviceProviders(serviceProviders), plugin(plugin),
          renderMode(MissedApproachRenderMode::None), renderForServiceProvision(Ownership::ServiceType::Invalid),
          renderDuration(0), DRAW_PEN(CreatePen())
    {
    }

    void MissedApproachRenderer::AsrLoadedEvent(Euroscope::UserSetting& userSetting)
    {
        // What to render
        auto asrRenderMode = userSetting.GetUnsignedIntegerEntry(
            this->RENDER_MODE_ASR_KEY, static_cast<unsigned int>(MissedApproachRenderMode::None));

        this->renderMode = asrRenderMode >= static_cast<unsigned int>(MissedApproachRenderMode::Invalid)
                               ? MissedApproachRenderMode::None
                               : static_cast<MissedApproachRenderMode>(asrRenderMode);

        // When to render
        auto asrServiceProvision = userSetting.GetUnsignedIntegerEntry(
            this->SERVICE_PROVISION_ASR_KEY, static_cast<unsigned int>(ServiceType::Invalid));

        this->renderForServiceProvision = asrServiceProvision >= static_cast<unsigned int>(ServiceType::Invalid)
                                              ? ServiceType::Invalid
                                              : static_cast<ServiceType>(asrServiceProvision);

        // How long to render
        this->renderDuration = std::chrono::seconds(
            userSetting.GetUnsignedIntegerEntry(this->SERVICE_PROVISION_ASR_KEY, DEFAULT_RENDER_DURATION));

        // Which airfields to render for
        this->renderFor = userSetting.GetStringListEntry(this->RENDER_FOR_ASR_KEY);
    }

    void MissedApproachRenderer::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
        userSetting.Save(
            this->RENDER_MODE_ASR_KEY, this->RENDER_MODE_ASR_DESC, static_cast<unsigned int>(this->renderMode));

        userSetting.Save(
            this->SERVICE_PROVISION_ASR_KEY,
            this->SERVICE_PROVISION_ASR_DESC,
            static_cast<unsigned int>(this->renderForServiceProvision));

        userSetting.Save(
            this->RENDER_DURATION_ASR_KEY,
            this->RENDER_DURATION_ASR_DESC,
            static_cast<unsigned int>(this->renderDuration.count()));

        userSetting.Save(this->RENDER_FOR_ASR_KEY, this->RENDER_FOR_ASR_DESC, this->renderFor);
    }

    auto MissedApproachRenderer::IsVisible() const -> bool
    {
        return this->renderMode != MissedApproachRenderMode::None;
    }

    void MissedApproachRenderer::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        const auto airfieldsProvidingServices =
            this->serviceProviders.GetAirfieldsWhereUserProvidingServices(this->renderForServiceProvision);

        std::vector<std::string> relevantAirfields;
        std::copy_if(
            airfieldsProvidingServices.cbegin(),
            airfieldsProvidingServices.cend(),
            std::back_inserter(relevantAirfields),
            [this](const std::string& airfield) -> bool {
                return std::find(this->renderFor.cbegin(), this->renderFor.cend(), airfield) != this->renderFor.cend();
            });

        if (relevantAirfields.empty()) {
            return;
        }

        this->missedApproaches->ForEach(
            [this, &relevantAirfields, &radarScreen, &graphics](const std::shared_ptr<class MissedApproach>& missed) {
                // Only display for certain duration
                if (TimeNow() > missed->CreatedAt() + this->renderDuration) {
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
                if ((this->renderMode & MissedApproachRenderMode::Line) == MissedApproachRenderMode::Line) {
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
                if ((this->renderMode & MissedApproachRenderMode::Circle) == MissedApproachRenderMode::Circle) {
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
        const EuroScopePlugIn::CPosition aircraftPosition,
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
