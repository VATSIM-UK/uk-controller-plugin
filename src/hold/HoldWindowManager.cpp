#include "pch/stdafx.h"
#include "hold/HoldWindowManager.h"
#include "hold/HoldDisplay.h"

namespace UKControllerPlugin {
    namespace Hold {

        HoldWindowManager::HoldWindowManager(HWND euroscopeWindow, HINSTANCE dllInstance)
            : euroscopeWindow(euroscopeWindow), dllInstance(dllInstance)
        {

        }

        void HoldWindowManager::AddWindow(void)
        {
            this->holdDisplays.emplace_back(euroscopeWindow, dllInstance);
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
