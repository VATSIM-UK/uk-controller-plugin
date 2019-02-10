#include "pch/stdafx.h"
#include "hold/HoldWindowManager.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldManager.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;

namespace UKControllerPlugin {
    namespace Hold {

        HoldWindowManager::HoldWindowManager(
            HWND euroscopeWindow,
            HINSTANCE dllInstance,
            const HoldManager & holdManager,
            EuroscopePluginLoopbackInterface & plugin
        )
            : euroscopeWindow(euroscopeWindow), dllInstance(dllInstance), holdManager(holdManager), plugin(plugin)
        {

        }

        void HoldWindowManager::AddWindow(void)
        {
            this->holdDisplays.emplace_back(euroscopeWindow, dllInstance, *this->holdManager.GetManagedHold(1));
        }

        /*
            Open up a popup menu displaying the available holds to add to
        */
        void HoldWindowManager::OpenHoldPopupMenu()
        {
            //this->plugin.Me
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
