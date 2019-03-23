#include "pch/stdafx.h"
#include "hold/HoldRenderer.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplay.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;

namespace UKControllerPlugin {
    namespace Hold {

        HoldRenderer::HoldRenderer(
            const HoldProfileManager & profileManager,
            const HoldManager & holdManager,
            const EuroscopePluginLoopbackInterface & plugin
        )
            : profileManager(profileManager), holdManager(holdManager), plugin(plugin)
        {

        }
        void HoldRenderer::AsrLoadedEvent(UserSetting & userSetting)
        {

        }

        void HoldRenderer::AsrClosingEvent(UserSetting & userSetting)
        {
        }

        bool HoldRenderer::IsVisible(void) const
        {
            return true;
        }

        void HoldRenderer::LeftClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {

        }

        void HoldRenderer::RightClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {

        }

        void HoldRenderer::Move(RECT position)
        {
        }

        void HoldRenderer::Render(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            HoldDisplay test(this->plugin, *this->holdManager.GetManagedHold(1));
            test.Move({ 100, 100 });
            test.PaintWindow(graphics);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
