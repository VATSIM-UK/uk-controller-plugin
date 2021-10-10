#include "MissedApproach.h"
#include "MissedApproachButton.h"
#include "MissedApproachCollection.h"
#include "TriggerMissedApproach.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"
#include "ownership/AirfieldServiceProviderCollection.h"

namespace UKControllerPlugin::MissedApproach {

    MissedApproachButton::MissedApproachButton(
        std::shared_ptr<const MissedApproachCollection> missedApproaches,
        std::shared_ptr<const TriggerMissedApproach> trigger,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        const Ownership::AirfieldServiceProviderCollection& serviceProviders,
        int screenObjectId)
        : missedApproaches(std::move(missedApproaches)), trigger(std::move(trigger)), plugin(plugin),
          serviceProviders(serviceProviders), screenObjectId(screenObjectId), isVisible(false),
          position(DEFAULT_POSITION), ACTIVE_BRUSH(ACTIVE_COLOR), DISABLED_BRUSH(DISABLED_COLOR),
          ARMED_BRUSH(ARMED_COLOR), TEXT_BRUSH(TEXT_COLOR), BORDER_PEN(BORDER_COLOR, BORDER_THICKNESS)
    {
    }

    void MissedApproachButton::AsrLoadedEvent(Euroscope::UserSetting& userSetting)
    {
    }

    void MissedApproachButton::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
    }

    auto MissedApproachButton::IsVisible() const -> bool
    {
        return this->isVisible;
    }

    void MissedApproachButton::LeftClick(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        RadarRenderableInterface::LeftClick(radarScreen, objectId, objectDescription, mousePos, itemArea);
    }

    void MissedApproachButton::Move(RECT position, std::string objectDescription)
    {
        this->position = position;
        this->renderRect =
            Gdiplus::Rect{position.left, position.top, position.right - position.left, position.bottom - position.top};
    }

    void MissedApproachButton::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        this->ApplyClickspot(radarScreen);

        const auto flightplan = this->plugin.GetSelectedFlightplan();
        if (!flightplan || !this->serviceProviders.TowerControlProvidedByUser(flightplan->GetDestination())) {
            this->RenderDisabledButton(graphics);
            return;
        }

        const auto& missedApproach = this->missedApproaches->Get(flightplan->GetCallsign());
        if (missedApproach) {
            this->RenderActiveButton(graphics);
            return;
        }

        this->RenderArmedButton(graphics);
    }

    void MissedApproachButton::ResetPosition()
    {
        this->Move(DEFAULT_POSITION, "");
    }

    void MissedApproachButton::RenderDisabledButton(Windows::GdiGraphicsInterface& graphics)
    {
        this->RenderBackground(graphics, DISABLED_BRUSH);
        this->RenderMissedApproachText(graphics);
        this->RenderBorder(graphics);
    }

    void MissedApproachButton::RenderBorder(Windows::GdiGraphicsInterface& graphics)
    {
        graphics.DrawRect(this->renderRect, BORDER_PEN);
    }

    void MissedApproachButton::RenderBackground(Windows::GdiGraphicsInterface& graphics, const Gdiplus::Brush& brush)
    {
        graphics.FillRect(this->renderRect, brush);
    }

    void MissedApproachButton::RenderMissedApproachText(Windows::GdiGraphicsInterface& graphics)
    {
        this->RenderText(graphics, L"GO AROUND");
    }

    void MissedApproachButton::RenderText(Windows::GdiGraphicsInterface& graphics, const std::wstring& text)
    {
        graphics.DrawString(text, this->renderRect, this->TEXT_BRUSH);
    }

    void MissedApproachButton::RenderActiveButton(Windows::GdiGraphicsInterface& graphics)
    {
        this->RenderBackground(graphics, ACTIVE_BRUSH);
        this->RenderMissedApproachText(graphics);
        this->RenderBorder(graphics);
    }

    void MissedApproachButton::RenderArmedButton(Windows::GdiGraphicsInterface& graphics)
    {
        this->RenderBackground(graphics, ARMED_BRUSH);
        this->RenderMissedApproachText(graphics);
        this->RenderBorder(graphics);
    }

    void MissedApproachButton::ApplyClickspot(Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        radarScreen.RegisterScreenObject(this->screenObjectId, "goAroundButton", this->position, true);
    }
} // namespace UKControllerPlugin::MissedApproach
