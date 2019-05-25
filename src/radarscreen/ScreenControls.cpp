#include "pch/stdafx.h"
#include "radarscreen/ScreenControls.h"
#include "graphics/GdiGraphicsInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "graphics/GdiplusBrushes.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using Gdiplus::Graphics;

namespace UKControllerPlugin {
    namespace RadarScreen {

        ScreenControls::ScreenControls(
            int toggleboxIdEuroscope,
            const ConfigurableDisplayCollection & configurableDisplays,
            const GdiplusBrushes & brushes
        )
            : controlWidth(25),
            controlHeight(25),
            toggleboxIdEuroscope(toggleboxIdEuroscope),
            configurableDisplays(configurableDisplays),
            brushes(brushes)
        {

        }

        /*
            Always visible.
        */
        bool ScreenControls::IsVisible(void) const
        {
            return true;
        }

        /*
            When someone clicks on the clickspot, pop up a menu with all the configuration options.
        */
        void ScreenControls::LeftClick(
            int objectId,
            std::string objectDescription,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            if (objectId != this->toggleboxIdEuroscope) {
                return;
            }

            RECT radarArea = radarScreen.GetRadarViewport();
            radarScreen.ToogleMenu({
                 radarArea.right - controlWidth, radarArea.bottom - controlHeight, radarArea.right, radarArea.bottom },
                 this->menuName,
                 1
                );

            for (
                ConfigurableDisplayCollection::const_iterator it = this->configurableDisplays.cbegin();
                it != this->configurableDisplays.cend();
                ++it
            ) {

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
            std::string objectDescription,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            this->LeftClick(objectId, objectDescription, radarScreen);
        }

        /*
            Renders the clickspot to the screen.
        */
        void ScreenControls::Render(
            UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            RECT radarArea = radarScreen.GetRadarViewport();
            Gdiplus::Rect renderArea = {
                radarArea.right - controlWidth,
                radarArea.bottom - controlHeight,
                controlWidth,
                controlHeight
            };
            graphics.FillRect(renderArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawString(L"OP", renderArea, *this->brushes.greenBrush);
            radarScreen.RegisterScreenObject(
                toggleboxIdEuroscope,
                "",
                {radarArea.right - controlWidth, radarArea.bottom - controlHeight, radarArea.right, radarArea.bottom},
                false
            );
        }
        
        /*
            This is always the bottom left of the screen, dont move.
        */
        void ScreenControls::ResetPosition(void)
        {
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
