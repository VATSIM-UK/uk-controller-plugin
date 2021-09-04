#include "AcknowledgePrenoteMessage.h"
#include "ComparePrenoteMessages.h"
#include "PendingPrenoteList.h"
#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeRadarLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "helper/HelperFunctions.h"
#include "components/TitleBar.h"
#include "components/Button.h"
#include "components/StandardButtons.h"
#include "components/ClickableArea.h"
#include "components/BrushSwitcher.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ActiveCallsignCollection.h"

namespace UKControllerPlugin::Prenote {

    PendingPrenoteList::PendingPrenoteList(
        std::shared_ptr<PrenoteMessageCollection> messages,
        std::shared_ptr<AcknowledgePrenoteMessage> acknowledger,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        const Controller::ControllerPositionCollection& controllers,
        const Controller::ActiveCallsignCollection& activeCallsigns,
        const int screenObjectId)
        : controllers(controllers), messages(std::move(messages)), acknowledger(std::move(acknowledger)),
          plugin(plugin), activeCallsigns(activeCallsigns), textBrush(OFF_WHITE_COLOUR), screenObjectId(screenObjectId),
          visible(false), contentCollapsed(false)
    {
        this->brushSwitcher = Components::BrushSwitcher::Create(
                                  std::make_shared<Gdiplus::SolidBrush>(TITLE_BAR_BASE_COLOUR), std::chrono::seconds(2))
                                  ->AdditionalBrush(std::make_shared<Gdiplus::SolidBrush>(TITLE_BAR_FLASH_COLOUR));

        this->titleBar = Components::TitleBar::Create(
                             L"Departure Release Requests", {0, 0, this->titleBarWidth, this->titleBarHeight})
                             ->WithDrag(this->screenObjectId)
                             ->WithBorder(std::make_shared<Gdiplus::Pen>(OFF_WHITE_COLOUR))
                             ->WithBackgroundBrush(std::make_shared<Gdiplus::SolidBrush>(TITLE_BAR_BASE_COLOUR))
                             ->WithTextBrush(std::make_shared<Gdiplus::SolidBrush>(OFF_WHITE_COLOUR));

        this->closeButton = Components::Button::Create(
            CLOSE_BUTTON_OFFSET, this->screenObjectId, "closeButton", Components::CloseButton());

        this->collapseButton = Components::Button::Create(
            COLLAPSE_BUTTON_OFFSET,
            this->screenObjectId,
            "collapseButton",
            Components::CollapseButton([this]() -> bool { return this->contentCollapsed; }));
    }

    void PendingPrenoteList::LeftClick(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        std::string objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        if (objectDescription == "collapseButton") {
            this->contentCollapsed = !this->contentCollapsed;
            return;
        }

        if (objectDescription == "closeButton") {
            this->visible = false;
            return;
        }

        auto fp = this->plugin.GetFlightplanForCallsign(objectDescription);
        if (!fp) {
            return;
        }

        this->acknowledger->Acknowledge(*fp);
    }

    void PendingPrenoteList::RightClick(
        int objectId, std::string objectDescription, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        // Do nothing
    }

    auto PendingPrenoteList::IsVisible() const -> bool
    {
        return this->visible;
    }

    void PendingPrenoteList::Move(RECT position, std::string objectDescription)
    {
        this->position = {static_cast<Gdiplus::REAL>(position.left), static_cast<Gdiplus::REAL>(position.top)};
    }

    void PendingPrenoteList::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        auto userControllerId = this->activeCallsigns.UserHasCallsign()
                                    ? this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId()
                                    : -1;

        std::set<std::shared_ptr<PrenoteMessage>, ComparePrenoteMessages> prenotes;
        if (userControllerId != -1) {
            this->messages->Iterate([&prenotes, &userControllerId](const std::shared_ptr<PrenoteMessage>& message) {
                if (message->GetTargetControllerId() == userControllerId) {
                    prenotes.insert(message);
                }
            });
        }

        if (prenotes.empty()) {
            this->titleBar->WithBackgroundBrush(this->brushSwitcher->Base());
        } else {
            this->titleBar->WithBackgroundBrush(this->brushSwitcher->Next());
        }

        // Translate to content position
        graphics.Translated(
            this->position.X,
            this->position.Y + static_cast<float>(this->titleBarHeight),
            [this, &graphics, &radarScreen, &prenotes] {
                if (this->contentCollapsed) {
                    return;
                }

                // Draw column headers
                graphics.DrawString(L"Callsign", this->callsignColumnHeader, this->textBrush);
                graphics.DrawString(L"Controller", this->controllerColumnHeader, this->textBrush);
                graphics.DrawString(L"Dept", this->airportColumnHeader, this->textBrush);
                graphics.DrawString(L"SID", this->sidColumnHeader, this->textBrush);
                graphics.DrawString(L"Dest", this->destColumnHeader, this->textBrush);

                // Draw each aircraft that we care about
                Gdiplus::Rect callsignColumn = this->callsignColumnHeader;
                Gdiplus::Rect controllerColumn = this->controllerColumnHeader;
                Gdiplus::Rect airportColumn = this->airportColumnHeader;
                Gdiplus::Rect sidColumn = this->sidColumnHeader;
                Gdiplus::Rect destColumn = this->destColumnHeader;

                // Draw each decision
                for (const auto& prenote : prenotes) {
                    auto fp = this->plugin.GetFlightplanForCallsign(prenote->GetCallsign());
                    if (!fp) {
                        continue;
                    }

                    // Shift the cols
                    callsignColumn.Y += LINE_HEIGHT;
                    controllerColumn.Y += LINE_HEIGHT;
                    airportColumn.Y += LINE_HEIGHT;
                    sidColumn.Y += LINE_HEIGHT;
                    destColumn.Y += LINE_HEIGHT;

                    graphics.DrawString(
                        HelperFunctions::ConvertToWideString(prenote->GetCallsign()), callsignColumn, this->textBrush);

                    std::shared_ptr<Components::ClickableArea> callsignClickspot = Components::ClickableArea::Create(
                        callsignColumn, this->screenObjectId, prenote->GetCallsign(), false);
                    callsignClickspot->Apply(graphics, radarScreen);

                    const std::wstring controller = HelperFunctions::ConvertToWideString(
                        this->controllers.FetchPositionById(prenote->GetSendingControllerId())->GetCallsign());
                    graphics.DrawString(controller, controllerColumn, this->textBrush);

                    graphics.DrawString(
                        HelperFunctions::ConvertToWideString(fp->GetOrigin()), airportColumn, this->textBrush);

                    graphics.DrawString(
                        HelperFunctions::ConvertToWideString(fp->GetSidName()), sidColumn, this->textBrush);

                    graphics.DrawString(
                        HelperFunctions::ConvertToWideString(fp->GetDestination()), destColumn, this->textBrush);
                }
            });

        // Translate to window position
        graphics.Translated(this->position.X, this->position.Y, [this, &graphics, &radarScreen] {
            this->titleBar->Draw(graphics, radarScreen);
            this->closeButton->Draw(graphics, radarScreen);
            this->collapseButton->Draw(graphics, radarScreen);
        });
    }

    void PendingPrenoteList::ResetPosition()
    {
        this->Move(DEFAULT_POSITION, "");
    }

    void PendingPrenoteList::AsrLoadedEvent(Euroscope::UserSetting& userSetting)
    {
        this->visible = userSetting.GetBooleanEntry(GetAsrKey("Visible"), false);

        this->contentCollapsed = userSetting.GetBooleanEntry(GetAsrKey("ContentCollapsed"), false);

        this->Move(
            {userSetting.GetIntegerEntry(GetAsrKey("XPosition"), DEFAULT_POSITION.left),
             userSetting.GetIntegerEntry(GetAsrKey("YPosition"), DEFAULT_POSITION.top),
             0,
             0},
            "");
    }

    void PendingPrenoteList::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
        userSetting.Save(GetAsrKey("Visible"), GetAsrDescription("Visible"), this->visible);

        userSetting.Save(GetAsrKey("ContentCollapsed"), GetAsrDescription("Content Collapsed"), this->contentCollapsed);

        userSetting.Save(GetAsrKey("XPosition"), GetAsrDescription("X Position"), static_cast<int>(this->position.X));

        userSetting.Save(GetAsrKey("YPosition"), GetAsrDescription("Y Position"), static_cast<int>(this->position.Y));
    }

    void PendingPrenoteList::ToggleVisible()
    {
        this->visible = !this->visible;
    }

    auto PendingPrenoteList::ContentCollapsed() const -> bool
    {
        return this->contentCollapsed;
    }

    auto PendingPrenoteList::Position() const -> Gdiplus::PointF
    {
        return this->position;
    }

    auto PendingPrenoteList::GetAsrKey(const std::string& item) -> std::string
    {
        return "pendingPrenoteMessageList" + item;
    }

    auto PendingPrenoteList::GetAsrDescription(const std::string& description) -> std::string
    {
        return "Pending Prenote List " + description;
    }
} // namespace UKControllerPlugin::Prenote
