#include "MissedApproach.h"
#include "MissedApproachButton.h"
#include "MissedApproachCollection.h"
#include "TriggerMissedApproach.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSetting.h"
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
        this->isVisible = userSetting.GetBooleanEntry(this->ASR_VISIBILITY_KEY, false);
        auto xpos = userSetting.GetIntegerEntry(this->ASR_X_POSITION_KEY, DEFAULT_COORDINATE);
        auto ypos = userSetting.GetIntegerEntry(this->ASR_Y_POSITION_KEY, DEFAULT_COORDINATE);
        this->Move({xpos, ypos, xpos + DEFAULT_WIDTH, ypos + DEFAULT_HEIGHT}, "");
    }

    void MissedApproachButton::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
        userSetting.Save(this->ASR_VISIBILITY_KEY, this->ASR_VISIBILITY_DESC, this->isVisible);
        userSetting.Save(this->ASR_X_POSITION_KEY, this->ASR_X_POSITION_DESC, this->position.left);
        userSetting.Save(this->ASR_Y_POSITION_KEY, this->ASR_Y_POSITION_DESC, this->position.top);
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
        const auto flightplan = this->plugin.GetSelectedFlightplan();
        const auto radarTarget = this->plugin.GetSelectedRadarTarget();
        if (!flightplan || !radarTarget) {
            return;
        }

        this->trigger->Trigger(*flightplan, *radarTarget);
    }

    void MissedApproachButton::Move(RECT position, std::string objectDescription)
    {
        this->position = {position.left, position.top, position.left + DEFAULT_WIDTH, position.top + DEFAULT_HEIGHT};
        this->renderRect = Gdiplus::Rect{this->position.left, this->position.top, DEFAULT_WIDTH, DEFAULT_HEIGHT};
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

    auto MissedApproachButton::Position() const -> RECT
    {
        return this->position;
    }
    
    void MissedApproachButton::ToggleVisible()
    {
        this->isVisible = !this->isVisible;
    }
} // namespace UKControllerPlugin::MissedApproach
