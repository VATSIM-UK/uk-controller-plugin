#pragma once
#include "minstack/MinStackRendererConfiguration.h"
#include "minstack/MinStackManager.h"

namespace UKControllerPlugin {
    namespace MinStack {

        /*
            A class for handling the dialog that configures the countdown timer.
        */
        class MinStackConfigurationDialog
        {
            public:
                MinStackConfigurationDialog(const UKControllerPlugin::MinStack::MinStackManager & manager);
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                void InitDialog(HWND hwnd, LPARAM lParam);
                void SaveDialog(HWND hwnd);

                // The renderer configuration this relates to
                UKControllerPlugin::MinStack::MinStackRendererConfiguration * const config = nullptr;

                // Manages MSLs
                const UKControllerPlugin::MinStack::MinStackManager & manager;
        };
    }  // namespace MinStack
}  // namespace UKControllerPlugin
