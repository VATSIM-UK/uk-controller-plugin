#pragma once
#include "minstack/MinStackManager.h"
#include "minstack/MinStackRendererConfiguration.h"

namespace UKControllerPlugin::MinStack {

    /*
        A class for handling the dialog that configures the countdown timer.
    */
    class MinStackConfigurationDialog
    {
        public:
        explicit MinStackConfigurationDialog(const UKControllerPlugin::MinStack::MinStackManager& manager);
        static auto CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

        private:
        auto InternalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
        static void AddEntryToActiveList(HWND hwnd);
        static void DoElementSwap(
            HWND hwnd,
            unsigned int firstItemIndex,
            LPARAM firstItemString,
            LPARAM firstItemData,
            unsigned int secondItemIndex,
            LPARAM secondItemString,
            LPARAM secondItemData);

        void InitDialog(HWND hwnd, LPARAM lParam);
        void SwapElements(HWND hwnd, bool swapUp);
        static void RemoveEntryFromActiveList(HWND hwnd);
        void SaveDialog(HWND hwnd);
        static auto GetListEntryForKey(const std::string& mslKey) -> std::wstring;
        static auto IsLastElement(unsigned int selectedIndex, unsigned int itemCount, bool swapUp) -> bool;

        // The renderer configuration this relates to
        UKControllerPlugin::MinStack::MinStackRendererConfiguration* config = nullptr;

        // Manages MSLs
        const UKControllerPlugin::MinStack::MinStackManager& manager;

        // The keys currently active for MSL whilst the dialog is open
        std::set<std::string> activeMslKeys;

        static const int AIRFIELD_SUBSTRING_LENGTH = 9;
        static const int TMA_SUBSTRING_LENGTH = 4;
    };
} // namespace UKControllerPlugin::MinStack
