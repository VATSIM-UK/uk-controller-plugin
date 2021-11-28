#include "srd/SrdSearchDialog.h"
#include "dialog/DialogCallArgument.h"
#include "srd/SrdSearchParameters.h"
#include "hold/HoldDisplayFunctions.h"
#include "helper/HelperFunctions.h"
#include "api/ApiException.h"
#include "datablock/DatablockFunctions.h"
#include "srd/ContainsFreeRouteAirspace.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Datablock::ConvertAltitudeToFlightLevel;
using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin {
    namespace Srd {
        SrdSearchDialog::SrdSearchDialog(const UKControllerPlugin::Api::ApiInterface& api) : api(api)
        {
        }
        /*
            Public facing window procedure
        */
        LRESULT SrdSearchDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("SRD search dialog opened");
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<DialogCallArgument*>(lParam)->dialogArgument);
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("SRD search dialog closed");
            }

            SrdSearchDialog* dialog = reinterpret_cast<SrdSearchDialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        bool SrdSearchDialog::SearchResultsValid(const nlohmann::json& results) const
        {
            if (!results.is_array()) {
                LogError("SRD search result is not an array" + results.dump());
                return false;
            }

            for (nlohmann::json::const_iterator it = results.cbegin(); it != results.cend(); ++it) {

                if (!it->contains("minimum_level") ||
                    (!it->at("minimum_level").is_number_integer() && !it->at("minimum_level").is_null())) {
                    LogError("SRD search result has invalid minimum level " + results.dump());
                    return false;
                }

                if (!it->contains("maximum_level") || !it->at("maximum_level").is_number_integer()) {
                    LogError("SRD search result has invalid maximum level " + results.dump());
                    return false;
                }

                if (!it->contains("route_string") || !it->at("route_string").is_string()) {
                    LogError("SRD search result has invalid route string " + results.dump());
                    return false;
                }

                if (!it->contains("notes") || !it->at("notes").is_array()) {
                    LogError("SRD search result has invalid notes array " + results.dump());
                    return false;
                }

                for (nlohmann::json::const_iterator noteIt = it->at("notes").cbegin(); noteIt != it->at("notes").cend();
                     ++noteIt) {

                    if (!noteIt->is_object() || !noteIt->contains("id") || !noteIt->at("id").is_number_integer() ||
                        !noteIt->contains("text") || !noteIt->at("text").is_string()) {
                        LogError("SRD search result has an invalid note " + results.dump());
                        return false;
                    }
                }
            }

            return true;
        }

        /*
            Format the notes for the SRD.
        */
        std::string SrdSearchDialog::FormatNotes(const nlohmann::json& json, size_t selectedIndex) const
        {
            if (json.size() == 0 || json.size() - 1 < selectedIndex) {
                LogWarning("Tried to access invalid selected route");
                return "Notes invalid.";
            }

            nlohmann::json selectedRoute = json.at(selectedIndex);
            std::string fraNotes = RouteContainsFreeRouteAirspace(selectedRoute.at("route_string").get<std::string>())
                                       ? "This route contains segments of Free Route Airspace."
                                       : "";
            if (!selectedRoute.contains("notes") || selectedRoute.at("notes").empty()) {
                return "No notes." + (fraNotes.empty() ? "" : "\r\n\r\n" + fraNotes);
            }

            std::string noteString;
            for (nlohmann::json::const_iterator noteIt = selectedRoute.at("notes").cbegin();
                 noteIt != selectedRoute.at("notes").cend();
                 ++noteIt) {
                noteString += "Note " + std::to_string(noteIt->at("id").get<int>()) + "\n\n" +
                              noteIt->at("text").get<std::string>() + "\n\n";
            }

            noteString += fraNotes;
            return std::regex_replace(noteString, std::regex("[\r\n]"), "\r\n");
            ;
        }

        /*
            Private window procedure bound to the objects
        */
        LRESULT SrdSearchDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
            // Initialise
            case WM_INITDIALOG: {
                this->InitDialog(hwnd, lParam);
                return TRUE;
            }
            // Dialog Closed
            case WM_CLOSE: {
                EndDialog(hwnd, wParam);
                return TRUE;
            }
            // Catching the events when search results are clicked.
            case WM_NOTIFY: {
                switch (((LPNMHDR)lParam)->code) {
                case LVN_ITEMCHANGED: {
                    if (((LPNMHDR)lParam)->idFrom == IDC_SRD_RESULTS) {
                        this->SelectSearchResult(hwnd, reinterpret_cast<NMLISTVIEW*>((LPNMHDR)lParam));
                        return TRUE;
                    }
                }
                default:
                    return FALSE;
                }
            }
            // Buttons pressed
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                case IDOK: {
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                case IDCANCEL: {
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                case IDC_SRD_SEARCH: {
                    this->StartSearch(hwnd);
                    return TRUE;
                }
                case IDC_SRD_ROUTE_COPY: {
                    this->CopyRouteStringToClipboard(hwnd);
                    return TRUE;
                }
                default:
                    return FALSE;
                }
            }
            }

            return FALSE;
        }

        void SrdSearchDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            HWND resultsList = GetDlgItem(hwnd, IDC_SRD_RESULTS);

            // Make items highlight on row select
            SendMessage(resultsList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

            // Create the min level column
            wchar_t min[4] = L"Min";
            LVCOLUMN minLevelColumn = {
                LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM, LVCFMT_LEFT | LVCFMT_FIXED_WIDTH, 50, min, 0};
            ListView_InsertColumn(resultsList, 0, &minLevelColumn);

            // Create the max level column
            wchar_t max[4] = L"Max";
            LVCOLUMN maxLevelColumn = {
                LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM, LVCFMT_LEFT | LVCFMT_FIXED_WIDTH, 50, max, 1};
            ListView_InsertColumn(resultsList, 1, &maxLevelColumn);

            // Create the Free Route Airspace column
            wchar_t fra[4] = L"FRA";
            LVCOLUMN freeRouteAirspaceColumn = {
                LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM, LVCFMT_LEFT | LVCFMT_FIXED_WIDTH, 50, fra, 1};
            ListView_InsertColumn(resultsList, 2, &freeRouteAirspaceColumn);

            // Create the route string column
            wchar_t routeString[13] = L"Route String";
            LVCOLUMN routeStringColumn = {
                LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
                LVCFMT_LEFT | LVCFMT_FIXED_WIDTH,
                335,
                routeString,
                2};
            ListView_InsertColumn(resultsList, 3, &routeStringColumn);
        }

        void SrdSearchDialog::StartSearch(HWND hwnd)
        {
            SrdSearchParameters searchParams;

            // Get the origin
            TCHAR originBuffer[255];
            SendDlgItemMessage(hwnd, IDC_SRD_ORIGIN, WM_GETTEXT, 255, reinterpret_cast<LPARAM>(&originBuffer));
            searchParams.origin = UKControllerPlugin::Hold::ConvertFromTchar(originBuffer);

            // Get the destination
            TCHAR destinationBuffer[255];
            SendDlgItemMessage(
                hwnd, IDC_SRD_DESTINATION, WM_GETTEXT, 255, reinterpret_cast<LPARAM>(&destinationBuffer));
            searchParams.destination = UKControllerPlugin::Hold::ConvertFromTchar(destinationBuffer);

            // Get the requested cruise level
            TCHAR cruiseBuffer[255];
            SendDlgItemMessage(hwnd, IDC_SRD_CRUISE, WM_GETTEXT, 255, reinterpret_cast<LPARAM>(&cruiseBuffer));
            std::string requestedLevel = UKControllerPlugin::Hold::ConvertFromTchar(cruiseBuffer);

            // If the requested level is < 100, assume it's a flight-level
            if (!requestedLevel.empty()) {
                searchParams.requestedLevel = std::stoi(requestedLevel);
                if (searchParams.requestedLevel < 1000) {
                    searchParams.requestedLevel *= 100;
                }
            }

            // Clear the results list and notes box
            HWND resultsList = GetDlgItem(hwnd, IDC_SRD_RESULTS);
            ListView_DeleteAllItems(resultsList);
            this->previousSearchResults = this->noResultsFound;
            SendDlgItemMessage(hwnd, IDC_SRD_NOTES, WM_SETTEXT, NULL, (LPARAM)L"");

            // Disable the route copy button
            EnableWindow(GetDlgItem(hwnd, IDC_SRD_ROUTE_COPY), false);

            if (resultsList == NULL) {
                return;
            }

            // Do the search and validate the results
            try {
                this->previousSearchResults = this->api.SearchSrd(searchParams);
            } catch (ApiException e) {
                LogError("Failed to perform SRD search: " + std::string(e.what()));
            }

            if (this->previousSearchResults.empty() || !this->SearchResultsValid(this->previousSearchResults)) {
                LVITEM item;
                item.mask = LVIF_TEXT;
                item.iItem = 0;
                item.iSubItem = 0;

                // Nothing found, display the fact
                wchar_t zero[2] = L"0";
                item.pszText = zero;
                ListView_InsertItem(resultsList, &item);
                item.iSubItem++;
                item.pszText = zero;
                ListView_SetItem(resultsList, &item);
                item.iSubItem++;
                item.pszText = zero;
                ListView_SetItem(resultsList, &item);
                item.iSubItem++;
                wchar_t noneFound[16] = L"No routes found";
                item.pszText = noneFound;
                ListView_SetItem(resultsList, &item);

                return;
            }

            // Populate the results list with results
            int itemNumber = 0;
            for (nlohmann::json::const_iterator it = this->previousSearchResults.cbegin();
                 it != this->previousSearchResults.cend();
                 ++it) {
                LVITEM item;
                item.mask = LVIF_TEXT;
                item.iItem = itemNumber;
                item.iSubItem = 0;

                // Min Level
                std::wstring minLevel =
                    it->at("minimum_level").is_null()
                        ? L"MC"
                        : std::to_wstring(ConvertAltitudeToFlightLevel(it->at("minimum_level").get<int>()));
                item.pszText = (LPWSTR)minLevel.c_str();
                ListView_InsertItem(resultsList, &item);

                // Max Level
                item.iSubItem++;
                std::wstring maxLevel =
                    std::to_wstring(ConvertAltitudeToFlightLevel(it->at("maximum_level").get<int>()));
                item.pszText = (LPWSTR)maxLevel.c_str();
                ListView_SetItem(resultsList, &item);

                // Free route airspace
                item.iSubItem++;
                std::wstring freeRoute =
                    RouteContainsFreeRouteAirspace(it->at("route_string").get<std::string>()) ? L"Yes" : L"No";
                item.pszText = (LPWSTR)freeRoute.c_str();
                ListView_SetItem(resultsList, &item);

                // Route String
                item.iSubItem++;
                std::wstring routeString =
                    HelperFunctions::ConvertToWideString(it->at("route_string").get<std::string>());
                item.pszText = (LPWSTR)(routeString.c_str());
                ListView_SetItem(resultsList, &item);

                itemNumber++;
            }
        }

        void SrdSearchDialog::CopyRouteStringToClipboard(HWND hwnd)
        {
            if (!OpenClipboard(hwnd)) {
                LogError("Failed to open clipboard: " + std::to_string(GetLastError()));
            }

            EmptyClipboard();

            std::string routeString =
                this->previousSearchResults.at(this->selectedResult).at("route_string").get<std::string>();

            HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE, (routeString.size() + 1) * sizeof(char));
            if (handle == NULL) {
                LogError("Failed to copy to clipboard: " + std::to_string(GetLastError()));
                return;
            }

            LPSTR copy = reinterpret_cast<LPSTR>(GlobalLock(handle));
            memcpy(copy, routeString.c_str(), (routeString.size() + 1) * sizeof(char));
            GlobalUnlock(copy);

            SetClipboardData(CF_TEXT, copy);
            CloseClipboard();
        }

        void SrdSearchDialog::SelectSearchResult(HWND hwnd, NMLISTVIEW* details)
        {
            // If a new item has been selected, change the notes
            if (((details->uNewState ^ details->uOldState) & LVIS_SELECTED) && !this->previousSearchResults.empty()) {

                this->selectedResult = details->iItem;

                std::wstring noteStringWide = HelperFunctions::ConvertToWideString(
                    this->FormatNotes(this->previousSearchResults, details->iItem));

                SendDlgItemMessage(hwnd, IDC_SRD_NOTES, WM_SETTEXT, NULL, (LPARAM)noteStringWide.c_str());

                // Enable the route copy button
                EnableWindow(GetDlgItem(hwnd, IDC_SRD_ROUTE_COPY), true);
            }
        }

    } // namespace Srd
} // namespace UKControllerPlugin
