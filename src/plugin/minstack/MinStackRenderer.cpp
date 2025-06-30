#include "MinStackLevel.h"
#include "MinStackManager.h"
#include "MinStackRenderer.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GdiplusBrushes.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::MinStack::MinStackRenderer;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin::MinStack {

    MinStackRenderer::MinStackRenderer(
        UKControllerPlugin::MinStack::MinStackManager& minStackModule,
        int closeClickspotId,
        int menuBarClickspotId,
        int mslClickspotId,
        int toggleCallbackFunctionId,
        const GdiplusBrushes& brushes,
        const UKControllerPlugin::Dialog::DialogManager& dialogManager)
        : brushes(brushes), minStackModule(minStackModule), dialogManager(dialogManager),
          hideClickspotId(closeClickspotId), menuBarClickspotId(menuBarClickspotId), mslClickspotId(mslClickspotId),
          toggleCallbackFunctionId(toggleCallbackFunctionId)
    {
    }

    /*
        Called when the ASR is first loaded to initialise important data.
    */
    void MinStackRenderer::AsrLoadedEvent(UserSetting& userSetting)
    {
        this->config.SetShouldRender(userSetting.GetBooleanEntry(this->visibleUserSettingKey, true));
        this->topBarArea.left = userSetting.GetIntegerEntry(this->xPositionUserSettingKey, DEFAULT_POSITION);
        this->topBarArea.top = userSetting.GetIntegerEntry(this->yPositionUserSettingKey, DEFAULT_POSITION);
        std::vector<std::string> selectedMinStacks = userSetting.GetStringListEntry(
            this->selectedMinStackUserSettingKey,
            {MinStackManager::GetMslKeyTma("LTMA"),
             MinStackManager::GetMslKeyTma("MTMA"),
             MinStackManager::GetMslKeyTma("STMA")});

        int order = 0;
        for (auto& selectedMinStack : selectedMinStacks) {
            this->config.AddItem({order++, selectedMinStack});
        }

        this->topBarArea.right = this->topBarArea.left + this->leftColumnWidth;
        this->topBarArea.bottom = this->topBarArea.top + this->rowHeight;

        this->hideClickspotArea = {
            this->topBarArea.right,
            this->topBarArea.top,
            this->topBarArea.right + this->hideClickspotWidth,
            this->topBarArea.bottom};

        this->topBarRender = {this->topBarArea.left, this->topBarArea.top, this->leftColumnWidth, this->rowHeight};
        this->hideSpotRender = {
            this->topBarArea.right, this->topBarArea.top, this->hideClickspotWidth, this->rowHeight};
    }

    /*
        Called when the ASR is closing. Saves important data to the ASR for next time.
    */
    void MinStackRenderer::AsrClosingEvent(UserSetting& userSetting)
    {
        userSetting.Save(this->visibleUserSettingKey, this->visibleUserSettingDescription, this->config.ShouldRender());
        userSetting.Save(this->xPositionUserSettingKey, this->xPositionUserSettingDescription, this->topBarArea.left);
        userSetting.Save(this->yPositionUserSettingKey, this->yPositionUserSettingDescription, this->topBarArea.top);

        std::vector<std::string> selectedMinStacks;
        for (const auto& configuredMinStack : this->config) {
            selectedMinStacks.push_back(configuredMinStack.key);
        }

        userSetting.Save(
            this->selectedMinStackUserSettingKey, this->selectedMinStackUserSettingDescription, selectedMinStacks);
    }

    /*
        Returns the menu configuration item.
    */
    auto MinStackRenderer::GetConfigurationMenuItem() const -> PopupMenuItem
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
    void MinStackRenderer::Configure(
        [[maybe_unused]] int functionId, [[maybe_unused]] std::string subject, [[maybe_unused]] RECT screenObjectArea)
    {
        this->dialogManager.OpenDialog(IDD_MINSTACK, reinterpret_cast<LPARAM>(&this->config)); // NOLINT
    }

    auto MinStackRenderer::GetConfig() -> MinStackRendererConfiguration&
    {
        return this->config;
    }

    /*
        Returns the screen area for the hide clickspot.
    */
    auto MinStackRenderer::GetHideClickspotArea() const -> RECT
    {
        return this->hideClickspotArea;
    }

    /*
        Returns the renderable area for the hide clickspot.
    */
    auto MinStackRenderer::GetHideSpotRender() const -> Gdiplus::Rect
    {
        return this->hideSpotRender;
    }

    /*
        Return the screen area for the top bar.
    */
    auto MinStackRenderer::GetTopBarArea() const -> RECT
    {
        return this->topBarArea;
    }

    /*
        Return the renderable area for the top bar.
    */
    auto MinStackRenderer::GetTopBarRender() const -> Gdiplus::Rect
    {
        return this->topBarRender;
    }

    /*
        Called when one of the clickable areas is clicked on.
    */
    void MinStackRenderer::LeftClick(
        EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        [[maybe_unused]] POINT mousePos,
        [[maybe_unused]] RECT itemArea)
    {
        // Hiding the module
        if (objectId == this->hideClickspotId) {
            this->config.SetShouldRender(false);
            return;
        }

        // It was the MSL that was clicked
        this->minStackModule.AcknowledgeMsl(objectDescription);
    }

    /*
        Returns whether or not the module is visible on the screen.
    */
    auto MinStackRenderer::IsVisible() const -> bool
    {
        return this->config.ShouldRender();
    }

    /*
        Moves the topleft point of the MSL window.
    */
    void MinStackRenderer::Move(RECT titleBarArea, [[maybe_unused]] std::string objectDescription)
    {
        this->topBarArea = titleBarArea;
        this->hideClickspotArea = {
            titleBarArea.right, titleBarArea.top, titleBarArea.right + this->hideClickspotWidth, titleBarArea.bottom};
        this->topBarRender = {this->topBarArea.left, this->topBarArea.top, this->leftColumnWidth, this->rowHeight};
        this->hideSpotRender = {
            this->topBarArea.right, this->topBarArea.top, this->hideClickspotWidth, this->rowHeight};
    }

    /*
        Function called to render the module to the screen.
    */
    void MinStackRenderer::Render(GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        this->RenderTopBar(graphics, radarScreen);
        this->RenderOuterFrame(graphics, radarScreen, this->RenderMinStacks(graphics, radarScreen));
    }

    /*
        Render the individual minimum stack levels and their associated clickspots.
    */
    auto MinStackRenderer::RenderMinStacks(
        GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen) const -> int
    {
        // Loop through each of the TMAs
        Gdiplus::Rect tma = {this->topBarArea.left, this->topBarArea.bottom, this->leftColumnWidth, this->rowHeight};
        Gdiplus::Rect msl = {
            this->topBarArea.right, this->topBarArea.bottom, this->hideClickspotWidth, this->rowHeight};

        int roundNumber = 0;
        for (const auto& minStack : this->config) {
            const MinStackLevel& mslData = this->minStackModule.GetMinStackLevel(minStack.key);

            // Draw the TMA title and rectangles
            graphics.FillRect(tma, Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(tma, Gdiplus::Pen(this->brushes.border));

            graphics.DrawString(
                HelperFunctions::ConvertToWideString(MinStackManager::GetNameFromKey(minStack.key)),
                tma,
                mslData.IsAcknowledged() ? Gdiplus::SolidBrush(this->brushes.acknowledge) : Gdiplus::SolidBrush(this->brushes.text));

            // Draw the MSL itself and associated rectangles
            graphics.FillRect(msl, Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(msl, Gdiplus::Pen(this->brushes.border));

            std::string mslString =
                mslData == this->minStackModule.InvalidMsl() ? "-" : std::to_string(mslData.msl).substr(0, 2);

            graphics.DrawString(
                HelperFunctions::ConvertToWideString(mslString),
                msl,
                mslData.IsAcknowledged() ? Gdiplus::SolidBrush(this->brushes.acknowledge) : Gdiplus::SolidBrush(this->brushes.text));

            // Add the clickable area.
            radarScreen.RegisterScreenObject(
                this->mslClickspotId,
                minStack.key,
                {tma.X, tma.Y, msl.X + this->hideClickspotWidth, msl.Y + this->rowHeight},
                false);

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
        GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen, int numMinStacks)
    {
        Gdiplus::Rect area = {
            this->topBarArea.left,
            this->topBarArea.top,
            this->leftColumnWidth + this->hideClickspotWidth,
            1 + ((numMinStacks) * this->rowHeight)};
        graphics.DrawRect(area, Gdiplus::Pen(this->brushes.border));
    }

    /*
        Renders the title bar of the MSL display.
    */
    void MinStackRenderer::RenderTopBar(GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        // The title bar - the draggable bit
        graphics.DrawRect(this->topBarRender, Gdiplus::Pen(this->brushes.border));
        graphics.FillRect(this->topBarRender, Gdiplus::SolidBrush(this->brushes.header));
        graphics.DrawString(L"MSL", this->topBarRender, Gdiplus::SolidBrush(this->brushes.text));
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
    void MinStackRenderer::RightClick(
        int objectId, const std::string& objectDescription, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        this->LeftClick(radarScreen, objectId, objectDescription, POINT{}, RECT{});
    }

    /*
        Reset the position of the renderer
    */
    void MinStackRenderer::ResetPosition()
    {
        this->Move(
            {DEFAULT_POSITION,
             DEFAULT_POSITION,
             DEFAULT_POSITION + this->leftColumnWidth,
             DEFAULT_POSITION + this->rowHeight},
            "");
    }

    void MinStackRenderer::SetVisible(bool visible)
    {
        this->config.SetShouldRender(visible);
    }

    auto MinStackRenderer::VisibleUserSettingKey() const -> const std::string&
    {
        return visibleUserSettingKey;
    }

    auto MinStackRenderer::SelectedMinStackUserSettingKey() const -> const std::string&
    {
        return selectedMinStackUserSettingKey;
    }

    auto MinStackRenderer::SelectedMinStackUserSettingDescription() const -> const std::string&
    {
        return selectedMinStackUserSettingDescription;
    }

    auto MinStackRenderer::VisibleUserSettingDescription() const -> const std::string&
    {
        return visibleUserSettingDescription;
    }

    auto MinStackRenderer::XPositionUserSettingKey() const -> const std::string&
    {
        return xPositionUserSettingKey;
    }

    auto MinStackRenderer::XPositionUserSettingDescription() const -> const std::string&
    {
        return xPositionUserSettingDescription;
    }

    auto MinStackRenderer::YPositionUserSettingKey() const -> const std::string&
    {
        return yPositionUserSettingKey;
    }

    auto MinStackRenderer::YPositionUserSettingDescription() const -> const std::string&
    {
        return yPositionUserSettingDescription;
    }

    auto MinStackRenderer::MenuItemDescription() const -> const std::string&
    {
        return menuItemDescription;
    }

    auto MinStackRenderer::HideClickspotId() const -> int
    {
        return hideClickspotId;
    }

    auto MinStackRenderer::MslClickspotId() const -> int
    {
        return mslClickspotId;
    }

    auto MinStackRenderer::LeftColumnWidth() const -> int
    {
        return leftColumnWidth;
    }

    auto MinStackRenderer::RowHeight() const -> int
    {
        return rowHeight;
    }

    auto MinStackRenderer::HideClickspotWidth() const -> int
    {
        return hideClickspotWidth;
    }
} // namespace UKControllerPlugin::MinStack
