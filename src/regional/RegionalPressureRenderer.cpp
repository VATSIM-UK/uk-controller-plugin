#include "pch/stdafx.h"
#include "regional/RegionalPressureRenderer.h"
#include "regional/RegionalPressureManager.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "helper/HelperFunctions.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Dialog::DialogManager;

namespace UKControllerPlugin {
    namespace Regional {

        RegionalPressureRenderer::RegionalPressureRenderer(
            RegionalPressureManager & manager,
            int closeClickspotId,
            int menuBarClickspotId,
            int mslClickspotId,
            int toggleCallbackFunctionId,
            const GdiplusBrushes & brushes,
            const UKControllerPlugin::Dialog::DialogManager & dialogManager
        )
            : manager(manager), hideClickspotId(closeClickspotId), menuBarClickspotId(menuBarClickspotId),
            mslClickspotId(mslClickspotId), leftColumnWidth(75), rowHeight(20), hideClickspotWidth(25),
            toggleCallbackFunctionId(toggleCallbackFunctionId), brushes(brushes), dialogManager(dialogManager)
        {

        }

        /*
            Called when the ASR is first loaded to initialise important data.
        */
        void RegionalPressureRenderer::AsrLoadedEvent(UserSetting & userSetting)
        {
            this->config.SetShouldRender(userSetting.GetBooleanEntry(this->visibleUserSettingKey, true));
            this->topBarArea.left = userSetting.GetIntegerEntry(this->xPositionUserSettingKey, 100);
            this->topBarArea.top = userSetting.GetIntegerEntry(this->yPositionUserSettingKey, 100);
            std::vector<std::string> selectedMinStacks = userSetting.GetStringListEntry(
                this->selectedRegionalPressureUserSettingKey,
                {
                    "ASR_LONDON",
                    "ASR_SCOTTISH",
                }
            );

            unsigned int order = 0;
            for (
                std::vector<std::string>::const_iterator it = selectedMinStacks.cbegin();
                it != selectedMinStacks.cend();
                ++it
            ) {
                this->config.AddItem(
                    {
                        order++,
                        *it
                    }
                );
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
        void RegionalPressureRenderer::AsrClosingEvent(UserSetting & userSetting)
        {
            userSetting.Save(
                this->visibleUserSettingKey,
                this->visibleUserSettingDescription,
                this->config.ShouldRender()
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

            std::vector<std::string> selectedPressures;
            for (
                RegionalPressureRendererConfiguration::const_iterator it = this->config.cbegin();
                it != this->config.cend();
                ++it
            ) {
                selectedPressures.push_back(it->key);
            }

            userSetting.Save(
                this->selectedRegionalPressureUserSettingKey,
                this->selecteRegionalPressureUserSettingDescription,
                selectedPressures
            );
        }

        /*
            Returns the menu configuration item.
        */
        PopupMenuItem RegionalPressureRenderer::GetConfigurationMenuItem(void) const
        {
            PopupMenuItem returnVal;
            returnVal.firstValue = this->menuItemDescription;
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->toggleCallbackFunctionId;
            returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        /*
            The configuration function called if the menu item is selected. It toggles the
            visibility.
        */
        void RegionalPressureRenderer::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->dialogManager.OpenDialog(
                IDD_MINSTACK,
                reinterpret_cast<LPARAM>(&this->config)
            );
        }

        RegionalPressureRendererConfiguration & RegionalPressureRenderer::GetConfig(void)
        {
            return this->config;
        }

        /*
            Returns the screen area for the hide clickspot.
        */
        RECT RegionalPressureRenderer::GetHideClickspotArea(void) const
        {
            return this->hideClickspotArea;
        }

        /*
            Returns the renderable area for the hide clickspot.
        */
        Gdiplus::Rect RegionalPressureRenderer::GetHideSpotRender(void) const
        {
            return this->hideSpotRender;
        }

        /*
            Return the screen area for the top bar.
        */
        RECT RegionalPressureRenderer::GetTopBarArea(void) const
        {
            return this->topBarArea;
        }

        /*
            Return the renderable area for the top bar.
        */
        Gdiplus::Rect RegionalPressureRenderer::GetTopBarRender(void) const
        {
            return this->topBarRender;
        }

        /*
            Called when one of the clickable areas is clicked on.
        */
        void RegionalPressureRenderer::LeftClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            // Hiding the module
            if (objectId == this->hideClickspotId) {
                this->config.SetShouldRender(false);
                return;
            }

            // It was the MSL that was clicked
            this->manager.AcknowledgePressure(objectDescription);
        }

        /*
            Returns whether or not the module is visible on the screen.
        */
        bool RegionalPressureRenderer::IsVisible(void) const
        {
            return this->config.ShouldRender();
        }

        /*
            Moves the topleft point of the MSL window.
        */
        void RegionalPressureRenderer::Move(RECT titleBarArea, std::string objectDescription)
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
        void RegionalPressureRenderer::Render(GdiGraphicsInterface & graphics, EuroscopeRadarLoopbackInterface & radarScreen)
        {
            this->RenderTopBar(graphics, radarScreen);
            this->RenderOuterFrame(graphics, radarScreen, this->RenderPressures(graphics, radarScreen));
        }

        /*
            Render the individual pressures and their associated clickspots.
        */
        int RegionalPressureRenderer::RenderPressures(
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
                RegionalPressureRendererConfiguration::const_iterator it = this->config.cbegin();
                it != this->config.cend();
                ++it
            ) {
                const RegionalPressure & pressureData = this->manager.GetRegionalPressure(it->key);

                // Draw the TMA title and rectangles
                graphics.FillRect(tma, *this->brushes.greyBrush);
                graphics.DrawRect(tma, *this->brushes.blackPen);

                graphics.DrawString(
                    HelperFunctions::ConvertToWideString(this->manager.GetNameFromKey(it->key)),
                    tma,
                    pressureData.IsAcknowledged() ? *this->brushes.whiteBrush : *this->brushes.yellowBrush
                );

                // Draw the MSL itself and associated rectangles
                graphics.FillRect(msl, *this->brushes.greyBrush);
                graphics.DrawRect(msl, *this->brushes.blackPen);

                std::string mslString = pressureData == this->manager.invalidPressure
                    ? "-"
                    : std::to_string(pressureData.pressure).substr(0, 2);

                graphics.DrawString(
                    HelperFunctions::ConvertToWideString(mslString),
                    msl,
                    pressureData.IsAcknowledged() ? *this->brushes.whiteBrush : *this->brushes.yellowBrush
                );

                // Add the clickable area.
                radarScreen.RegisterScreenObject(
                    this->mslClickspotId,
                    it->key,
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
        void RegionalPressureRenderer::RenderOuterFrame(
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
        void RegionalPressureRenderer::RenderTopBar(
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
        void RegionalPressureRenderer::RightClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            this->LeftClick(objectId, objectDescription, radarScreen);
        }

        /*
            Reset the position of the renderer
        */
        void RegionalPressureRenderer::ResetPosition(void)
        {
            this->Move(
                { 100, 100, 100 + this->leftColumnWidth, 100 + this->rowHeight },
                ""
            );
        }

        void RegionalPressureRenderer::SetVisible(bool visible)
        {
            this->config.SetShouldRender(visible);
        }
    }  // namespace MinStack
}  // namespace UKControllerPlugin
