#include "pch/stdafx.h"
#include "hold/HoldWindowManager.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldManager.h"

using UKControllerPlugin::Hold::HoldManager;

namespace UKControllerPlugin {
    namespace Hold {

        HoldWindowManager::HoldWindowManager(
            HWND euroscopeWindow,
            HINSTANCE dllInstance,
            const HoldManager & holdManager
        )
            : euroscopeWindow(euroscopeWindow), dllInstance(dllInstance), holdManager(holdManager)
        {

        }

        void HoldWindowManager::AddWindow(void)
        {
            this->holdDisplays.emplace_back(euroscopeWindow, dllInstance, *this->holdManager.GetManagedHold(1));
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
