#pragma once

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace IntentionCode {
        class AircraftFirExitGenerator;
    } // namespace IntentionCode
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Srd {

    /*
        A class for performing SRD searches
    */
    class SrdSearchDialog
    {
        public:
        SrdSearchDialog(
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const UKControllerPlugin::Api::ApiInterface& api,
            IntentionCode::AircraftFirExitGenerator& firExitGenerator);
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        bool SearchResultsValid(const nlohmann::json& results) const;
        std::string FormatNotes(const nlohmann::json& json, size_t selectedIndex) const;

        private:
        LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        void InitDialog(HWND hwnd, LPARAM lParam);
        void StartSearch(HWND hwnd);
        void CopyRouteStringToClipboard(HWND hwnd);
        void SelectSearchResult(HWND hwnd, NMLISTVIEW* details);
        void PrepopulateSearch(HWND hwnd, LPARAM lParam);

        // Plugin for flightplan finding
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The API for making searches
        const UKControllerPlugin::Api::ApiInterface& api;

        // For prepopulating UK exit points
        IntentionCode::AircraftFirExitGenerator& firExitGenerator;

        // No results found JSON
        const nlohmann::json noResultsFound = nlohmann::json::array();

        // The JSON result of the previous search
        nlohmann::json previousSearchResults = noResultsFound;

        // The selected search result
        size_t selectedResult = 0;
    };
} // namespace UKControllerPlugin::Srd
