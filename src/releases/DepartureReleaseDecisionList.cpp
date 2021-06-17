#include "pch/stdafx.h"
#include "releases/DepartureReleaseDecisionList.h"
#include "releases/DepartureReleaseRequest.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "helper/HelperFunctions.h"
#include "components/TitleBar.h"
#include "components/Button.h"
#include "components/StandardButtons.h"
#include "components/ClickableArea.h"

namespace UKControllerPlugin {

    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope

    namespace Releases {

        DepartureReleaseDecisionList::DepartureReleaseDecisionList(
            DepartureReleaseEventHandler& handler,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const int screenObjectId
        ): handler(handler), plugin(plugin), textBrush(Gdiplus::Color(227, 227, 227)), screenObjectId(screenObjectId),
           visible(false),
           contentCollapsed(false)
        {
            this->titleBar = Components::TitleBar::Create(
                                 L"Departure Release Requests",
                                 {0, 0, this->titleBarWidth, this->titleBarHeight}
                             )
                             ->WithDrag(this->screenObjectId)
                             ->WithBorder(std::make_shared<Gdiplus::Pen>(Gdiplus::Color(227, 227, 227)))
                             ->WithBackgroundBrush(std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(197, 129, 214)))
                             ->WithTextBrush(std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(227, 227, 227)));

            this->closeButton = Components::Button::Create(
                {385, 5, 10, 10},
                this->screenObjectId,
                "closeButton",
                Components::CloseButton()
            );

            this->collapseButton = Components::Button::Create(
                {370, 5, 10, 10},
                this->screenObjectId,
                "collapseButton",
                Components::CollapseButton([this]() -> bool { return this->contentCollapsed; })
            );
        }

        void DepartureReleaseDecisionList::LeftClick(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            std::string objectDescription,
            POINT mousePos,
            RECT itemArea
        )
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
            auto rt = this->plugin.GetRadarTargetForCallsign(objectDescription);
            if (!fp || rt) {
                return;
            }

            this->handler.OpenDecisionMenu(*fp, *rt, "", mousePos);
        }

        void DepartureReleaseDecisionList::RightClick(
            int objectId, std::string objectDescription,
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen
        )
        {
            // Do nothing
        }

        bool DepartureReleaseDecisionList::IsVisible() const
        {
            return this->visible;
        }

        void DepartureReleaseDecisionList::Move(RECT position, std::string objectDescription)
        {
            this->position = {
                static_cast<Gdiplus::REAL>(position.left),
                static_cast<Gdiplus::REAL>(position.top)
            };
        }

        void DepartureReleaseDecisionList::Render(
            Windows::GdiGraphicsInterface& graphics,
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen
        )
        {
            // Translate to content position
            graphics.Translated(
                this->position.X,
                this->position.Y + this->titleBarHeight,
                [this, &graphics, &radarScreen]
                {
                    if (this->contentCollapsed) {
                        return;
                    }

                    // Draw column headers
                    graphics.DrawString(L"Controller", this->callsignColumnHeader, this->textBrush);
                    graphics.DrawString(L"Dept", this->airportColumnHeader, this->textBrush);
                    graphics.DrawString(L"SID", this->sidColumnHeader, this->textBrush);

                    // Draw each aircraft that we care about
                    Gdiplus::Rect callsignColumn = this->callsignColumnHeader;
                    callsignColumn.Y += 25;
                    Gdiplus::Rect airportColumn = this->airportColumnHeader;
                    airportColumn.Y += 25;
                    Gdiplus::Rect sidColumn = this->sidColumnHeader;
                    sidColumn.Y += 25;

                    // Draw each decision
                    for (const auto decision : this->handler.GetReleasesRequiringUsersDecision()) {
                        auto fp = this->plugin.GetFlightplanForCallsign(decision->Callsign());
                        if (!fp) {
                            continue;
                        }

                        graphics.DrawString(
                            HelperFunctions::ConvertToWideString(decision->Callsign()),
                            callsignColumn,
                            this->textBrush
                        );
                        std::shared_ptr<Components::ClickableArea> callsignClickspot =
                            Components::ClickableArea::Create(
                                callsignColumn,
                                this->screenObjectId,
                                decision->Callsign(),
                                false
                            );
                        callsignClickspot->Apply(graphics, radarScreen);

                        graphics.DrawString(
                            HelperFunctions::ConvertToWideString(fp->GetOrigin()),
                            airportColumn,
                            this->textBrush
                        );

                        graphics.DrawString(
                            HelperFunctions::ConvertToWideString(fp->GetSidName()),
                            sidColumn,
                            this->textBrush
                        );
                    }
                }
            );

            // Translate to window position
            graphics.Translated(
                this->position.X,
                this->position.Y,
                [this, &graphics, &radarScreen]
                {
                    this->titleBar->Draw(graphics, radarScreen);
                    this->closeButton->Draw(graphics, radarScreen);
                    this->collapseButton->Draw(graphics, radarScreen);
                }
            );
        }

        void DepartureReleaseDecisionList::ResetPosition()
        {
            this->Move({100, 100, 200, 200}, "");
        }

        void DepartureReleaseDecisionList::AsrLoadedEvent(
            Euroscope::UserSetting& userSetting
        )
        {
            this->visible = userSetting.GetBooleanEntry(
                GetAsrKey("Visible"),
                false
            );

            this->contentCollapsed = userSetting.GetBooleanEntry(
                GetAsrKey("ContentCollapsed"),
                false
            );

            this->Move(
                {
                    userSetting.GetIntegerEntry(
                        GetAsrKey("XPosition"),
                        100
                    ),
                    userSetting.GetIntegerEntry(
                        GetAsrKey("YPosition"),
                        100
                    ),
                    0,
                    0
                },
                ""
            );
        }

        void DepartureReleaseDecisionList::AsrClosingEvent(
            Euroscope::UserSetting& userSetting
        )
        {
            userSetting.Save(
                GetAsrKey("Visible"),
                GetAsrDescription("Visible"),
                this->visible
            );

            userSetting.Save(
                GetAsrKey("ContentCollapsed"),
                GetAsrDescription("Content Collapsed"),
                this->contentCollapsed
            );

            userSetting.Save(
                GetAsrKey("XPosition"),
                GetAsrDescription("X Position"),
                this->position.X
            );

            userSetting.Save(
                GetAsrKey("YPosition"),
                GetAsrDescription("Y Position"),
                this->position.Y
            );
        }

        void DepartureReleaseDecisionList::ToggleVisible()
        {
            this->visible = !this->visible;
        }

        std::string DepartureReleaseDecisionList::GetAsrKey(std::string item)
        {
            return "departureReleaseRequestList" + item;
        }

        std::string DepartureReleaseDecisionList::GetAsrDescription(std::string description)
        {
            return "Departure Release Request List " + description;
        }
    } // namespace Releases
} // namespace UKControllerPlugin
