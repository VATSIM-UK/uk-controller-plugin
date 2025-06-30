#include "ConfigurableDisplayInterface.h"
#include "ScreenControls.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GdiplusBrushes.h"

using Gdiplus::Graphics;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin::RadarScreen {

    ScreenControls::ScreenControls(
        int toggleboxIdEuroscope, ConfigurableDisplayCollection configurableDisplays, const GdiplusBrushes& brushes)
        : brushes(brushes), configurableDisplays(std::move(configurableDisplays)),
          toggleboxIdEuroscope(toggleboxIdEuroscope)
    {
    }

    /*
        Always visible.
    */
    auto ScreenControls::IsVisible() const -> bool
    {
        return true;
    }

    /*
        When someone clicks on the clickspot, pop up a menu with all the configuration options.
    */
    void ScreenControls::LeftClick(
        EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        if (objectId != this->toggleboxIdEuroscope) {
            return;
        }

        RECT radarArea = radarScreen.GetRadarViewport();
        radarScreen.ToogleMenu(
            {radarArea.right - controlWidth, radarArea.bottom - controlHeight, radarArea.right, radarArea.bottom},
            this->menuName,
            1);

        for (auto it = this->configurableDisplays.cbegin(); it != this->configurableDisplays.cend(); ++it) {
            UKControllerPlugin::Plugin::PopupMenuItem item = (*it)->GetConfigurationMenuItem();
            radarScreen.AddMenuItem((*it)->GetConfigurationMenuItem());
        }
    }

    /*
        Doesn't move.
    */
    void ScreenControls::Move(RECT position, std::string objectDescription)
    {
    }

    /*
        Doesn't do right clicks.
    */
    void ScreenControls::RightClick(
        int objectId,
        const std::string& objectDescription,
        UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        this->LeftClick(radarScreen, objectId, objectDescription, POINT{}, RECT{});
    }

    /*
        Renders the clickspot to the screen.
    */
    void ScreenControls::Render(
        UKControllerPlugin::Windows::GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        RECT radarArea = radarScreen.GetRadarViewport();
        Gdiplus::Rect renderArea = {
            radarArea.right - controlWidth, radarArea.bottom - controlHeight, controlWidth, controlHeight};
        graphics.FillRect(renderArea, Gdiplus::SolidBrush(this->brushes.euroscopeBackground));
        graphics.DrawString(L"OP", renderArea, Gdiplus::SolidBrush(this->brushes.green));
        radarScreen.RegisterScreenObject(
            toggleboxIdEuroscope,
            "",
            {radarArea.right - controlWidth, radarArea.bottom - controlHeight, radarArea.right, radarArea.bottom},
            false);
    }

    /*
        This is always the bottom left of the screen, dont move.
    */
    void ScreenControls::ResetPosition()
    {
    }
} // namespace UKControllerPlugin::RadarScreen
