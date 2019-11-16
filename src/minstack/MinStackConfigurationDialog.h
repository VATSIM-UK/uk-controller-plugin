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
                explicit MinStackConfigurationDialog(const UKControllerPlugin::MinStack::MinStackManager & manager);
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                void AddEntryToActiveList(HWND hwnd, LPARAM lParam);
                void DoElementSwap(
                    HWND hwnd,
                    unsigned int firstItemIndex,
                    LPARAM firstItemString,
                    LPARAM firstItemData,
                    unsigned int secondItemIndex,
                    LPARAM secondItemString,
                    LPARAM secondItemData
                );

                void InitDialog(HWND hwnd, LPARAM lParam);
                void SwapElements(HWND hwnd, LPARAM lParam, bool swapUp);
                void RemoveEntryFromActiveList(HWND hwnd, LPARAM lParam);
                void SaveDialog(HWND hwnd);
                std::wstring GetListEntryForKey(std::string mslKey);
                bool IsLastElement(unsigned int selectedIndex, unsigned int itemCount, bool swapUp);

                // The renderer configuration this relates to
                UKControllerPlugin::MinStack::MinStackRendererConfiguration * config = nullptr;

                // Manages MSLs
                const UKControllerPlugin::MinStack::MinStackManager & manager;

                // The keys currently active for MSL whilst the dialog is open
                std::set<std::string> activeMslKeys;
        };
    }  // namespace MinStack
}  // namespace UKControllerPlugin
