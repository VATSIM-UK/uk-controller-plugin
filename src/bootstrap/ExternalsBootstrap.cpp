#include "pch/stdafx.h"
#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlApi.h"
#include "windows/WinApi.h"
#include "graphics/GdiplusBrushes.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "bootstrap/LocateApiSettings.h"
#include "euroscope/GeneralSettingsDialog.h"
#include "dialog/DialogManager.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Curl::CurlApi;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Windows::WinApi;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Windows::GdiGraphicsWrapper;
using UKControllerPlugin::Euroscope::GeneralSettingsDialog;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Bootstraps the external services and also some Gdiplus resources.
        */
        void ExternalsBootstrap::Bootstrap(PersistenceContainer & persistence, HINSTANCE instance)
        {
            persistence.curl.reset(new CurlApi());
            // All files should be relative to Documents/EuroScope
            std::unique_ptr<WinApi> winApi = std::make_unique<WinApi>(
                instance,
                ExternalsBootstrap::GetPluginFileRoot()
            );
            persistence.dialogManager.reset(new DialogManager(*winApi));
            persistence.windows = std::move(winApi);
            persistence.brushes.reset(new GdiplusBrushes);
            persistence.graphics.reset(new GdiGraphicsWrapper);
        }

        /*
            Return the path to UKCP settings files location in wide format
        */
        std::wstring ExternalsBootstrap::GetPluginFileRoot(void)
        {
            return ExternalsBootstrap::GetMyDocumentsPath() + L"/EuroScope/ukcp";
        }

        /*
            Create the required folder hierarchy for UKCP.
        */
        void ExternalsBootstrap::SetupUkcpFolderRoot(WinApiInterface & winApi)
        {
            std::wstring documentsPath = ExternalsBootstrap::GetMyDocumentsPath();
            if (!winApi.CreateFolderRecursive(documentsPath + L"/EuroScope/ukcp")) {
                winApi.OpenMessageBox(
                    L"Unable to create the UKCP root folder, please contact the VATUK Web Department.",
                    L"UKCP Fatal Error",
                    MB_OK | MB_ICONSTOP
                );
                throw std::runtime_error("Unable to create UKCP Root");
            }

            if (!winApi.SetPermissions(documentsPath + L"/EuroScope/ukcp", std::filesystem::perms::all)) {
                winApi.OpenMessageBox(
                    L"Unable to set permissions on the UKCP root folder, please contact the VATUK Web Department.",
                    L"UKCP Fatal Error",
                    MB_OK | MB_ICONSTOP
                );
                throw std::runtime_error("Unable to set permissions on the UKCP root folder");
            }
        }

        /*
            Get the My Documents path
        */
        std::wstring ExternalsBootstrap::GetMyDocumentsPath(void)
        {
            TCHAR * myDocumentsPath = 0;
            HRESULT result = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_SIMPLE_IDLIST, NULL, &myDocumentsPath);

            std::wstring widePath(myDocumentsPath);
            std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
            CoTaskMemFree(myDocumentsPath);
            return widePath;
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
