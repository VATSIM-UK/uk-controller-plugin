#include "RegionalPressureManager.h"
#include "RegionalPressureRenderer.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GdiplusBrushes.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin::Regional {

    RegionalPressureRenderer::RegionalPressureRenderer(
        RegionalPressureManager& manager,
        int closeClickspotId,
        int menuBarClickspotId,
        int rpsClickspotId,
        int toggleCallbackFunctionId,
        const GdiplusBrushes& brushes,
        const UKControllerPlugin::Dialog::DialogManager& dialogManager)
        : brushes(brushes), manager(manager), dialogManager(dialogManager), hideClickspotId(closeClickspotId),
          menuBarClickspotId(menuBarClickspotId), rpsClickspotId(rpsClickspotId),
          toggleCallbackFunctionId(toggleCallbackFunctionId)
    {
    }

    /*
        Called when the ASR is first loaded to initialise important data.
    */
    void RegionalPressureRenderer::AsrLoadedEvent(UserSetting& userSetting)
    {
        this->config.SetShouldRender(userSetting.GetBooleanEntry(this->visibleUserSettingKey, true));
        this->topBarArea.left = userSetting.GetIntegerEntry(this->xPositionUserSettingKey, DEFAULT_POSITION);
        this->topBarArea.top = userSetting.GetIntegerEntry(this->yPositionUserSettingKey, DEFAULT_POSITION);
        std::vector<std::string> selectedMinStacks = userSetting.GetStringListEntry(
            this->selectedRegionalPressureUserSettingKey,
            {
                "ASR_LONDON",
                "ASR_SCOTTISH",
            });

        int order = 0;
        for (auto it = selectedMinStacks.cbegin(); it != selectedMinStacks.cend(); ++it) {
            this->config.AddItem({order++, *it});
        }

        this->topBarArea.right = this->topBarArea.left + LEFT_COLUMN_WIDTH;
        this->topBarArea.bottom = this->topBarArea.top + ROW_HEIGHT;

        this->hideClickspotArea = {
            this->topBarArea.right,
            this->topBarArea.top,
            this->topBarArea.right + HIDE_CLICKSPOT_WIDTH,
            this->topBarArea.bottom};

        this->topBarRender = {this->topBarArea.left, this->topBarArea.top, LEFT_COLUMN_WIDTH, ROW_HEIGHT};
        this->hideSpotRender = {this->topBarArea.right, this->topBarArea.top, HIDE_CLICKSPOT_WIDTH, ROW_HEIGHT};
    }

    /*
        Called when the ASR is closing. Saves important data to the ASR for next time.
    */
    void RegionalPressureRenderer::AsrClosingEvent(UserSetting& userSetting)
    {
        userSetting.Save(this->visibleUserSettingKey, this->visibleUserSettingDescription, this->config.ShouldRender());
        userSetting.Save(this->xPositionUserSettingKey, this->xPositionUserSettingDescription, this->topBarArea.left);
        userSetting.Save(this->yPositionUserSettingKey, this->yPositionUserSettingDescription, this->topBarArea.top);

        std::vector<std::string> selectedPressures;
        for (auto it = this->config.cbegin(); it != this->config.cend(); ++it) {
            selectedPressures.push_back(it->key);
        }

        userSetting.Save(
            this->selectedRegionalPressureUserSettingKey,
            this->selecteRegionalPressureUserSettingDescription,
            selectedPressures);
    }

    /*
        Returns the menu configuration item.
    */
    auto RegionalPressureRenderer::GetConfigurationMenuItem() const -> PopupMenuItem
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
            IDD_REGIONAL_PRESSURE,
            reinterpret_cast<LPARAM>(&this->config) // NOLINT
        );
    }

    auto RegionalPressureRenderer::GetConfig() -> RegionalPressureRendererConfiguration&
    {
        return this->config;
    }

    /*
        Returns the screen area for the hide clickspot.
    */
    auto RegionalPressureRenderer::GetHideClickspotArea() const -> RECT
    {
        return this->hideClickspotArea;
    }

    /*
        Returns the renderable area for the hide clickspot.
    */
    auto RegionalPressureRenderer::GetHideSpotRender() const -> Gdiplus::Rect
    {
        return this->hideSpotRender;
    }

    /*
        Return the screen area for the top bar.
    */
    auto RegionalPressureRenderer::GetTopBarArea() const -> RECT
    {
        return this->topBarArea;
    }

    /*
        Return the renderable area for the top bar.
    */
    auto RegionalPressureRenderer::GetTopBarRender() const -> Gdiplus::Rect
    {
        return this->topBarRender;
    }

    /*
        Called when one of the clickable areas is clicked on.
    */
    void RegionalPressureRenderer::LeftClick(
        EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        // Hiding the module
        if (objectId == this->hideClickspotId) {
            this->config.SetShouldRender(false);
            return;
        }

        // It was the RPS that was clicked
        this->manager.AcknowledgePressure(objectDescription);
    }

    /*
        Returns whether or not the module is visible on the screen.
    */
    auto RegionalPressureRenderer::IsVisible() const -> bool
    {
        return this->config.ShouldRender();
    }

    /*
        Moves the topleft point of the RPS window.
    */
    void RegionalPressureRenderer::Move(RECT titleBarArea, std::string objectDescription)
    {
        this->topBarArea = titleBarArea;
        this->hideClickspotArea = {
            titleBarArea.right, titleBarArea.top, titleBarArea.right + HIDE_CLICKSPOT_WIDTH, titleBarArea.bottom};
        this->topBarRender = {this->topBarArea.left, this->topBarArea.top, LEFT_COLUMN_WIDTH, ROW_HEIGHT};
        this->hideSpotRender = {this->topBarArea.right, this->topBarArea.top, HIDE_CLICKSPOT_WIDTH, ROW_HEIGHT};
    }

    /*
        Function called to render the module to the screen.
    */
    void RegionalPressureRenderer::Render(GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        this->RenderTopBar(graphics, radarScreen);
        this->RenderOuterFrame(graphics, radarScreen, this->RenderPressures(graphics, radarScreen));
    }

    /*
        Render the individual pressures and their associated clickspots.
    */
    auto RegionalPressureRenderer::RenderPressures(
        GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen) -> int
    {
        // Loop through each of the TMAs
        Gdiplus::Rect asr = {this->topBarArea.left, this->topBarArea.bottom, LEFT_COLUMN_WIDTH, ROW_HEIGHT};
        Gdiplus::Rect rps = {this->topBarArea.right, this->topBarArea.bottom, HIDE_CLICKSPOT_WIDTH, ROW_HEIGHT};

        int roundNumber = 0;
        for (auto it = this->config.cbegin(); it != this->config.cend(); ++it) {
            const RegionalPressure& pressureData = this->manager.GetRegionalPressure(it->key);

            // Draw the TMA title and rectangles
            graphics.FillRect(asr, Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(asr, Gdiplus::Pen(this->brushes.border));

            graphics.DrawString(
                HelperFunctions::ConvertToWideString(this->manager.GetNameFromKey(it->key)),
                asr,
                pressureData.IsAcknowledged() ? Gdiplus::SolidBrush(this->brushes.acknowledge) : Gdiplus::SolidBrush(this->brushes.text));

            // Draw the RPS itself and associated rectangles
            graphics.FillRect(rps, Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(rps, Gdiplus::Pen(this->brushes.border));

            std::string rpsString;
            if (pressureData == this->manager.invalidPressure) {
                rpsString = "-";
            } else if (pressureData.pressure < APPEND_ZERO_LIMIT) {
                rpsString = "0" + std::to_string(pressureData.pressure);
            } else {
                rpsString = std::to_string(pressureData.pressure);
            }

            graphics.DrawString(
                HelperFunctions::ConvertToWideString(rpsString),
                rps,
                pressureData.IsAcknowledged() ? Gdiplus::SolidBrush(this->brushes.acknowledge) : Gdiplus::SolidBrush(this->brushes.text));

            // Add the clickable area.
            radarScreen.RegisterScreenObject(
                this->rpsClickspotId, it->key, {asr.X, asr.Y, rps.X + HIDE_CLICKSPOT_WIDTH, rps.Y + ROW_HEIGHT}, false);

            // Increment values for the next TMA
            asr.Y = asr.Y + ROW_HEIGHT;
            rps.Y = rps.Y + ROW_HEIGHT;
            roundNumber++;
        }

        return roundNumber;
    }

    /*
        Renders a frame around the box.
    */
    void RegionalPressureRenderer::RenderOuterFrame(
        GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen, int numRegionalPressures)
    {
        Gdiplus::Rect area = {
            this->topBarArea.left,
            this->topBarArea.top,
            LEFT_COLUMN_WIDTH + HIDE_CLICKSPOT_WIDTH,
            1 + ((numRegionalPressures)*ROW_HEIGHT)};
        graphics.DrawRect(area, Gdiplus::Pen(this->brushes.border));
    }

    /*
        Renders the title bar of the RPS display.
    */
    void
    RegionalPressureRenderer::RenderTopBar(GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        // The title bar - the draggable bit
        graphics.DrawRect(this->topBarRender, Gdiplus::Pen(this->brushes.border));
        graphics.FillRect(this->topBarRender, Gdiplus::SolidBrush(this->brushes.header));
        graphics.DrawString(L"ASR", this->topBarRender, Gdiplus::SolidBrush(this->brushes.text));
        radarScreen.RegisterScreenObject(this->menuBarClickspotId, "", this->topBarArea, true);

        // The toggle button - no draggable
        graphics.DrawRect(this->hideSpotRender, Gdiplus::Pen(this->brushes.border));
        graphics.FillRect(this->hideSpotRender, Gdiplus::SolidBrush(this->brushes.header));
        graphics.DrawString(L"X", this->hideSpotRender, Gdiplus::SolidBrush(this->brushes.text));
        radarScreen.RegisterScreenObject(this->hideClickspotId, "", this->hideClickspotArea, false);
    }

    /*
        When one the clickspots is clicked on. Just do whatever the leftclick does.
    */
    void RegionalPressureRenderer::RightClick(
        int objectId, const std::string& objectDescription, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        this->LeftClick(radarScreen, objectId, objectDescription, POINT{}, RECT{});
    }

    /*
        Reset the position of the renderer
    */
    void RegionalPressureRenderer::ResetPosition()
    {
        this->Move(
            {DEFAULT_POSITION, DEFAULT_POSITION, DEFAULT_POSITION + LEFT_COLUMN_WIDTH, DEFAULT_POSITION + ROW_HEIGHT},
            "");
    }

    void RegionalPressureRenderer::SetVisible(bool visible)
    {
        this->config.SetShouldRender(visible);
    }
} // namespace UKControllerPlugin::Regional
