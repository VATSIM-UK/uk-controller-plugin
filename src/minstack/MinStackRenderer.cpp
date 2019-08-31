#include "pch/stdafx.h"
#include "minstack/MinStackRenderer.h"
#include "minstack/MinStackManager.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "helper/HelperFunctions.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GdiplusBrushes.h"
#include "minstack/TerminalControlArea.h"

using UKControllerPlugin::MinStack::MinStackRenderer;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin {
    namespace MinStack {

        MinStackRenderer::MinStackRenderer(
            UKControllerPlugin::MinStack::MinStackManager & minStackModule,
            int closeClickspotId,
            int menuBarClickspotId,
            int mslClickspotId,
            int toggleCallbackFunctionId,
            const GdiplusBrushes & brushes
        )
            : minStackModule(minStackModule), hideClickspotId(closeClickspotId), menuBarClickspotId(menuBarClickspotId),
            mslClickspotId(mslClickspotId), leftColumnWidth(75), rowHeight(20), hideClickspotWidth(25),
            toggleCallbackFunctionId(toggleCallbackFunctionId), brushes(brushes)
        {

        }

        /*
            Called when the ASR is first loaded to initialise important data.
        */
        void MinStackRenderer::AsrLoadedEvent(UserSetting & userSetting)
        {
            this->visible = userSetting.GetBooleanEntry(this->visibleUserSettingKey, true);
            this->topBarArea.left = userSetting.GetIntegerEntry(this->xPositionUserSettingKey, 100);
            this->topBarArea.top = userSetting.GetIntegerEntry(this->yPositionUserSettingKey, 100);
            std::vector<std::string> selectedMinStacks = userSetting.GetStringListEntry(
                this->selectedMinStackUserSettingKey,
                {
                    this->minStackModule.GetMslKeyTma("LTMA"),
                    this->minStackModule.GetMslKeyTma("MTMA"),
                    this->minStackModule.GetMslKeyTma("STMA")
                }
            );

            for (
                std::vector<std::string>::const_iterator it = selectedMinStacks.cbegin();
                it != selectedMinStacks.cend();
                ++it
            ) {
                this->selectedMinStacks.insert(*it);
            }

            this->topBarArea.right = this->topBarArea.left + this->leftColumnWidth;
            this->topBarArea.bottom = this->topBarArea.top + this->rowHeight;

            this->hideClickspotArea = {
                this->topBarArea.right,
                this->topBarArea.top,
                this->topBarArea.right + this->hideClickspotWidth,
                this->topBarArea.bottom
            };

            this->topBarRender = {
                this->topBarArea.left,
                this->topBarArea.top,
                this->leftColumnWidth,
                this->rowHeight
            };
            this->hideSpotRender = {
                this->topBarArea.right,
                this->topBarArea.top,
                this->hideClickspotWidth,
                this->rowHeight
            };
        }

        /*
            Called when the ASR is closing. Saves important data to the ASR for next time.
        */
        void MinStackRenderer::AsrClosingEvent(UserSetting & userSetting)
        {
            userSetting.Save(
                this->visibleUserSettingKey,
                this->visibleUserSettingDescription,
                this->visible
            );
            userSetting.Save(
                this->xPositionUserSettingKey,
                this->xPositionUserSettingDescription,
                this->topBarArea.left
            );
            userSetting.Save(
                this->yPositionUserSettingKey,
                this->yPositionUserSettingDescription,
                this->topBarArea.top
            );

            std::vector<std::string> selectedMinStacks;
            for (
                std::set<std::string>::const_iterator it = this->selectedMinStacks.cbegin();
                it != this->selectedMinStacks.cend();
                ++it
            ) {
                selectedMinStacks.push_back(*it);
            }

            userSetting.Save(
                this->selectedMinStackUserSettingKey,
                this->selectedMinStackUserSettingDescription,
                selectedMinStacks
            );
        }

        /*
            Returns the menu configuration item.
        */
        PopupMenuItem MinStackRenderer::GetConfigurationMenuItem(void) const
        {
            PopupMenuItem returnVal;
            returnVal.firstValue = this->menuItemDescription;
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->toggleCallbackFunctionId;
            returnVal.checked = this->visible;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        /*
            The configuration function called if the menu item is selected. It toggles the
            visibility.
        */
        void MinStackRenderer::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->visible = !this->visible;
        }

        /*
            Returns the screen area for the hide clickspot.
        */
        RECT MinStackRenderer::GetHideClickspotArea(void) const
        {
            return this->hideClickspotArea;
        }

        /*
            Returns the renderable area for the hide clickspot.
        */
        Gdiplus::Rect MinStackRenderer::GetHideSpotRender(void) const
        {
            return this->hideSpotRender;
        }

        /*
            Return the screen area for the top bar.
        */
        RECT MinStackRenderer::GetTopBarArea(void) const
        {
            return this->topBarArea;
        }

        /*
            Return the renderable area for the top bar.
        */
        Gdiplus::Rect MinStackRenderer::GetTopBarRender(void) const
        {
            return this->topBarRender;
        }

        /*
            Called when one of the clickable areas is clicked on.
        */
        void MinStackRenderer::LeftClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            // Hiding the module
            if (objectId == this->hideClickspotId) {
                this->visible = false;
                return;
            }

            // It was the MSL that was clicked
            this->minStackModule.AcknowledgeMsl(objectDescription);
        }

        /*
            Returns whether or not the module is visible on the screen.
        */
        bool MinStackRenderer::IsVisible(void) const
        {
            return this->visible;
        }

        /*
            Moves the topleft point of the MSL window.
        */
        void MinStackRenderer::Move(RECT titleBarArea, std::string objectDescription)
        {
            this->topBarArea = titleBarArea;
            this->hideClickspotArea = {
                titleBarArea.right,
                titleBarArea.top,
                titleBarArea.right + this->hideClickspotWidth,
                titleBarArea.bottom
            };
            this->topBarRender = {
                this->topBarArea.left,
                this->topBarArea.top,
                this->leftColumnWidth,
                this->rowHeight
            };
            this->hideSpotRender = {
                this->topBarArea.right,
                this->topBarArea.top,
                this->hideClickspotWidth,
                this->rowHeight
            };
        }

        /*
            Function called to render the module to the screen.
        */
        void MinStackRenderer::Render(GdiGraphicsInterface & graphics, EuroscopeRadarLoopbackInterface & radarScreen)
        {
            this->RenderTopBar(graphics, radarScreen);
            this->RenderOuterFrame(graphics, radarScreen, this->RenderMinStacks(graphics, radarScreen));
        }

        /*
            Render the individual minimum stack levels and their associated clickspots.
        */
        int MinStackRenderer::RenderMinStacks(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            // Loop through each of the TMAs
            Gdiplus::Rect tma = {
                this->topBarArea.left,
                this->topBarArea.bottom,
                this->leftColumnWidth,
                this->rowHeight
            };
            Gdiplus::Rect msl = {
                this->topBarArea.right,
                this->topBarArea.bottom,
                this->hideClickspotWidth,
                this->rowHeight
            };

            int roundNumber = 0;
            for (
                std::set<std::string>::const_iterator it = this->selectedMinStacks.begin();
                it != selectedMinStacks.end();
                ++it
            ) {
                const MinStackLevel & mslData = this->minStackModule.GetMinStackLevel(*it);

                // Draw the TMA title and rectangles
                graphics.FillRect(tma, *this->brushes.greyBrush);
                graphics.DrawRect(tma, *this->brushes.blackPen);

                graphics.DrawString(
                    HelperFunctions::ConvertToWideString(this->minStackModule.GetNameFromKey(*it)),
                    tma,
                    mslData.IsAcknowledged() ? *this->brushes.whiteBrush : *this->brushes.yellowBrush
                );

                // Draw the MSL itself and associated rectangles
                graphics.FillRect(msl, *this->brushes.greyBrush);
                graphics.DrawRect(msl, *this->brushes.blackPen);

                std::string mslString = mslData == this->minStackModule.invalidMsl ? "-" : std::to_string(mslData.msl).substr(0, 2);

                graphics.DrawString(
                    HelperFunctions::ConvertToWideString(mslString),
                    msl,
                    mslData.IsAcknowledged() ? *this->brushes.whiteBrush : *this->brushes.yellowBrush
                );

                // Add the clickable area.
                radarScreen.RegisterScreenObject(
                    this->mslClickspotId,
                    *it,
                    {
                        tma.X,
                        tma.Y,
                        msl.X + this->hideClickspotWidth,
                        msl.Y + this->rowHeight
                    },
                    false
                );

                // Increment values for the next TMA
                tma.Y = tma.Y + this->rowHeight;
                msl.Y = msl.Y + this->rowHeight;
                roundNumber++;
            }

            return roundNumber;
        }

        /*
            Renders a frame around the box.
        */
        void MinStackRenderer::RenderOuterFrame(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen,
            int numMinStacks
        ) {
            Gdiplus::Rect area = {
                this->topBarArea.left,
                this->topBarArea.top,
                this->leftColumnWidth + this->hideClickspotWidth,
                1 + ((numMinStacks) * this->rowHeight)
            };
            graphics.DrawRect(
                area,
                *this->brushes.blackPen
            );
        }

        /*
            Renders the title bar of the MSL display.
        */
        void MinStackRenderer::RenderTopBar(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            // The title bar - the draggable bit
            graphics.DrawRect(this->topBarRender, *this->brushes.blackPen);
            graphics.FillRect(this->topBarRender, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawString(L"MSL", this->topBarRender, *this->brushes.whiteBrush);
            radarScreen.RegisterScreenObject(
                this->menuBarClickspotId,
                "",
                this->topBarArea,
                true
            );

            // The toggle button - no draggable
            graphics.DrawRect(this->hideSpotRender, *this->brushes.blackPen);
            graphics.FillRect(this->hideSpotRender, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawString(
                L"X",
                this->hideSpotRender,
                *this->brushes.whiteBrush
            );
            radarScreen.RegisterScreenObject(
                this->hideClickspotId,
                "",
                this->hideClickspotArea,
                false
            );
        }

        /*
            When one the clickspots is clicked on. Just do whatever the leftclick does.
        */
        void MinStackRenderer::RightClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            this->LeftClick(objectId, objectDescription, radarScreen);
        }

        void MinStackRenderer::SetSelectedMinStacks(std::set<std::string> selectedMinStacks)
        {
            this->selectedMinStacks = selectedMinStacks;
        }

        /*
            Sets the visibility of the renderer.
        */
        void MinStackRenderer::SetVisible(bool visible)
        {
            this->visible = visible;
        }

        /*
            Reset the position of the renderer
        */
        void MinStackRenderer::ResetPosition(void)
        {
            this->Move(
                { 100, 100, 100 + this->leftColumnWidth, 100 + this->rowHeight },
                ""
            );
        }
    }  // namespace MinStack
}  // namespace UKControllerPlugin
