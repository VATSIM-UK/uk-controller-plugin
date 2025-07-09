#include "DepartureCoordinationList.h"
#include "components/BrushSwitcher.h"
#include "components/Button.h"
#include "components/ClickableArea.h"
#include "components/StandardButtons.h"
#include "components/TitleBar.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "helper/HelperFunctions.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "releases/DepartureReleaseRequest.h"
#include "tag/TagData.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin::Departure {

    DepartureCoordinationList::DepartureCoordinationList(
        Releases::DepartureReleaseEventHandler& handler,
        Prenote::PrenoteMessageCollection& prenotes,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        const Controller::ControllerPositionCollection& controllers,
        const Controller::ActiveCallsignCollection& activeCallsigns,
        const GdiplusBrushes& brushes,
        const int screenObjectId)
        : controllers(controllers), handler(handler), prenotes(prenotes), brushes(brushes), plugin(plugin),
          activeCallsigns(activeCallsigns), screenObjectId(screenObjectId), visible(false),
          contentCollapsed(false)
    {
        this->brushSwitcher = Components::BrushSwitcher::Create(
                                  std::make_shared<Gdiplus::SolidBrush>(this->brushes.header), std::chrono::seconds(2))
                                  ->AdditionalBrush(std::make_shared<Gdiplus::SolidBrush>(this->brushes.highlightedHeader));

        this->titleBar = Components::TitleBar::Create(
                             L"Departure Coordination Requests", {0, 0, this->titleBarWidth, this->titleBarHeight})
                             ->WithDrag(this->screenObjectId)
                             ->WithBorder(std::make_shared<Gdiplus::Pen>(this->brushes.border))
                             ->WithBackgroundBrush(std::make_shared<Gdiplus::SolidBrush>(this->brushes.header))
                             ->WithTextBrush(std::make_shared<Gdiplus::SolidBrush>(this->brushes.text));

        this->closeButton = Components::Button::Create(
            closeButtonOffset, this->screenObjectId, "closeButton", Components::CloseButton());

        this->collapseButton = Components::Button::Create(
            collapseButtonOffset, this->screenObjectId, "collapseButton", Components::CollapseButton([this]() -> bool {
                return this->contentCollapsed;
            }));
    }

    void DepartureCoordinationList::LeftClick(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
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

        const std::string callsign = objectDescription.substr(4);
        objectDescription.substr(0, 3) == "Rls"
            ? radarScreen.TogglePluginTagFunction(
                  callsign, DEPARTURE_RELEASE_DECISION_TAG_FUNCTION_ID, mousePos, itemArea)
            : radarScreen.TogglePluginTagFunction(callsign, ACKNOWLEDGE_PRENOTE_TAG_FUNCTION_ID, mousePos, itemArea);
    }

    auto DepartureCoordinationList::IsVisible() const -> bool
    {
        return this->visible;
    }

    void DepartureCoordinationList::Move(RECT position, std::string objectDescription)
    {
        this->position = {static_cast<Gdiplus::REAL>(position.left), static_cast<Gdiplus::REAL>(position.top)};
    }

    void DepartureCoordinationList::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        auto decisions = this->handler.GetReleasesRequiringUsersDecision();
        auto userControllerId = this->activeCallsigns.UserHasCallsign()
                                    ? this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId()
                                    : -1;

        std::vector<std::shared_ptr<Prenote::PrenoteMessage>> prenoteMessages;
        if (userControllerId != -1) {
            this->prenotes.Iterate(
                [&prenoteMessages, &userControllerId](const std::shared_ptr<Prenote::PrenoteMessage>& message) {
                    if (message->GetTargetControllerId() == userControllerId && !message->IsAcknowledged()) {
                        prenoteMessages.push_back(message);
                    }
                });
        }

        if (decisions.empty() && prenoteMessages.empty()) {
            this->titleBar->WithBackgroundBrush(this->brushSwitcher->Base());
        } else {
            this->titleBar->WithBackgroundBrush(this->brushSwitcher->Next());
        }

        // Translate to content position
        graphics.Translated(
            this->position.X,
            this->position.Y + static_cast<float>(this->titleBarHeight),
            [this, &graphics, &radarScreen, &decisions, &prenoteMessages] {
                if (this->contentCollapsed) {
                    return;
                }
                // Draw column headers
                graphics.DrawString(L"Type", this->typeColumnHeader, Gdiplus::SolidBrush(this->brushes.text));
                graphics.DrawString(L"Callsign", this->callsignColumnHeader, Gdiplus::SolidBrush(this->brushes.text));
                graphics.DrawString(L"Controller", this->controllerColumnHeader, Gdiplus::SolidBrush(this->brushes.text));
                graphics.DrawString(L"Dept", this->airportColumnHeader, Gdiplus::SolidBrush(this->brushes.text));
                graphics.DrawString(L"SID", this->sidColumnHeader, Gdiplus::SolidBrush(this->brushes.text));
                graphics.DrawString(L"Dest", this->destColumnHeader, Gdiplus::SolidBrush(this->brushes.text));

                // Draw each aircraft that we care about
                Gdiplus::Rect typeColumn = this->typeColumnHeader;
                Gdiplus::Rect callsignColumn = this->callsignColumnHeader;
                Gdiplus::Rect controllerColumn = this->controllerColumnHeader;
                Gdiplus::Rect airportColumn = this->airportColumnHeader;
                Gdiplus::Rect sidColumn = this->sidColumnHeader;
                Gdiplus::Rect destColumn = this->destColumnHeader;

                auto nextRelease = decisions.cbegin();
                auto nextPrenote = prenoteMessages.cbegin();
                std::variant<
                    std::shared_ptr<Releases::DepartureReleaseRequest>,
                    std::shared_ptr<Prenote::PrenoteMessage>>
                    listItem;

                do {
                    // Out of things to draw
                    if (nextRelease == decisions.cend() && nextPrenote == prenoteMessages.cend()) {
                        break;
                    }

                    // Chose the next item in the list - this is, broadly, ordered by created at time
                    if (nextRelease == decisions.cend()) {
                        listItem = std::variant<
                            std::shared_ptr<Releases::DepartureReleaseRequest>,
                            std::shared_ptr<Prenote::PrenoteMessage>>(*nextPrenote++);
                    } else if (nextPrenote == prenoteMessages.cend()) {
                        listItem = std::variant<
                            std::shared_ptr<Releases::DepartureReleaseRequest>,
                            std::shared_ptr<Prenote::PrenoteMessage>>(*nextRelease++);
                    } else if ((*nextPrenote)->GetCreatedAt() < (*nextRelease)->CreatedAt()) {
                        listItem = std::variant<
                            std::shared_ptr<Releases::DepartureReleaseRequest>,
                            std::shared_ptr<Prenote::PrenoteMessage>>(*nextPrenote++);
                    } else {
                        listItem = std::variant<
                            std::shared_ptr<Releases::DepartureReleaseRequest>,
                            std::shared_ptr<Prenote::PrenoteMessage>>(*nextRelease++);
                    }

                    // Shift the cols
                    typeColumn.Y += lineHeight;
                    callsignColumn.Y += lineHeight;
                    controllerColumn.Y += lineHeight;
                    airportColumn.Y += lineHeight;
                    sidColumn.Y += lineHeight;
                    destColumn.Y += lineHeight;

                    // Type column
                    const std::string itemType = listItem.index() == 0 ? "Rls" : "Pre";
                    graphics.DrawString(HelperFunctions::ConvertToWideString(itemType), typeColumn, Gdiplus::SolidBrush(this->brushes.text));

                    // Callsign column
                    const std::string callsign =
                        listItem.index() == 0
                            ? std::get<std::shared_ptr<Releases::DepartureReleaseRequest>>(listItem)->Callsign()
                            : std::get<std::shared_ptr<Prenote::PrenoteMessage>>(listItem)->GetCallsign();
                    graphics.DrawString(
                        HelperFunctions::ConvertToWideString(callsign), callsignColumn, Gdiplus::SolidBrush(this->brushes.text));
                    std::shared_ptr<Components::ClickableArea> callsignClickspot = Components::ClickableArea::Create(
                        callsignColumn, this->screenObjectId, itemType + "." + callsign, false);
                    callsignClickspot->Apply(graphics, radarScreen);

                    // Controller column
                    const int controllerId =
                        listItem.index() == 0
                            ? std::get<std::shared_ptr<Releases::DepartureReleaseRequest>>(listItem)
                                  ->RequestingController()
                            : std::get<std::shared_ptr<Prenote::PrenoteMessage>>(listItem)->GetSendingControllerId();
                    const std::wstring controller = HelperFunctions::ConvertToWideString(
                        this->controllers.FetchPositionById(controllerId)->GetCallsign());
                    graphics.DrawString(controller, controllerColumn, Gdiplus::SolidBrush(this->brushes.text));

                    auto fp = this->plugin.GetFlightplanForCallsign(callsign);
                    if (!fp) {
                        continue;
                    }

                    // Remaining FP-driven columns
                    graphics.DrawString(
                        HelperFunctions::ConvertToWideString(fp->GetOrigin()), airportColumn, Gdiplus::SolidBrush(this->brushes.text));

                    graphics.DrawString(
                        HelperFunctions::ConvertToWideString(fp->GetSidName()), sidColumn, Gdiplus::SolidBrush(this->brushes.text));

                    graphics.DrawString(
                        HelperFunctions::ConvertToWideString(fp->GetDestination()), destColumn, Gdiplus::SolidBrush(this->brushes.text));
                } while (nextRelease != decisions.cend() || nextPrenote != prenoteMessages.cend());
            });

        // Translate to window position
        graphics.Translated(this->position.X, this->position.Y, [this, &graphics, &radarScreen] {
            this->titleBar->DrawTheme(graphics, radarScreen, brushes);
            this->closeButton->Draw(graphics, radarScreen);
            this->collapseButton->Draw(graphics, radarScreen);
        });
    }

    void DepartureCoordinationList::ResetPosition()
    {
        this->Move(defaultRect, "");
    }

    void DepartureCoordinationList::AsrLoadedEvent(Euroscope::UserSetting& userSetting)
    {
        this->visible = userSetting.GetBooleanEntry(GetAsrKey("Visible"), false);

        this->contentCollapsed = userSetting.GetBooleanEntry(GetAsrKey("ContentCollapsed"), false);

        this->Move(
            {userSetting.GetIntegerEntry(GetAsrKey("XPosition"), defaultPosition),
             userSetting.GetIntegerEntry(GetAsrKey("YPosition"), defaultPosition),
             0,
             0},
            "");
    }

    void DepartureCoordinationList::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
        userSetting.Save(GetAsrKey("Visible"), GetAsrDescription("Visible"), this->visible);

        userSetting.Save(GetAsrKey("ContentCollapsed"), GetAsrDescription("Content Collapsed"), this->contentCollapsed);

        userSetting.Save(GetAsrKey("XPosition"), GetAsrDescription("X Position"), static_cast<int>(this->position.X));

        userSetting.Save(GetAsrKey("YPosition"), GetAsrDescription("Y Position"), static_cast<int>(this->position.Y));
    }

    void DepartureCoordinationList::ToggleVisible()
    {
        this->visible = !this->visible;
    }

    auto DepartureCoordinationList::ContentCollapsed() const -> bool
    {
        return this->contentCollapsed;
    }

    auto DepartureCoordinationList::Position() const -> Gdiplus::PointF
    {
        return this->position;
    }

    auto DepartureCoordinationList::GetAsrKey(const std::string& item) -> std::string
    {
        return "departureCoordinationList" + item;
    }

    auto DepartureCoordinationList::GetAsrDescription(const std::string& description) -> std::string
    {
        return "Departure Coordination List " + description;
    }
} // namespace UKControllerPlugin::Departure
