#pragma once
#include "api/ApiInterface.h"

namespace UKControllerPlugin {
    namespace Srd {

        /*
            A class for performing SRD searches
        */
        class SrdSearchDialog
        {
            public:
                explicit SrdSearchDialog(const UKControllerPlugin::Api::ApiInterface& api);
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                bool SearchResultsValid(const nlohmann::json results) const;

            private:
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                void InitDialog(HWND hwnd, LPARAM lParam);
                void StartSearch(HWND hwnd);

                // The API for making searches
                const UKControllerPlugin::Api::ApiInterface& api;
        };
    }  // namespace Srd
}  // UKControllerPlugin
