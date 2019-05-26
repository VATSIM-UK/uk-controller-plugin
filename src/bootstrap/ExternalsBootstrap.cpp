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
                ExternalsBootstrap::GetPluginFileRoot(),
                ExternalsBootstrap::GetPluginFileRootWide()
            );
            persistence.dialogManager.reset(new DialogManager(*winApi));
            persistence.windows = std::move(winApi);
            persistence.brushes.reset(new GdiplusBrushes);
            persistence.graphics.reset(new GdiGraphicsWrapper);
        }

        /*
            Return the path to UKCP settings files location in narrow format
        */
        std::string ExternalsBootstrap::GetPluginFileRoot(void)
        {
            return HelperFunctions::ConvertToRegularString(GetPluginFileRootWide());
        }

        /*
            Return the path to UKCP settings files location in wide format
        */
        std::wstring ExternalsBootstrap::GetPluginFileRootWide(void)
        {
            return ExternalsBootstrap::GetMyDocumentsPath() + L"/EuroScope/ukcp";
        }

        /*
            Create the required folder hierarchy for UKCP.
        */
        void ExternalsBootstrap::SetupUkcpFolderRoot(WinApiInterface & winApi)
        {
            std::string documentsPath = HelperFunctions::ConvertToRegularString(
                ExternalsBootstrap::GetMyDocumentsPath()
            );

            if (!winApi.CreateFolderRecursive(documentsPath + "/EuroScope/ukcp")) {
                winApi.OpenMessageBox(
                    L"Unable to create the UKCP root folder, please contact the VATUK Web Department.",
                    L"UKCP Fatal Error",
                    MB_OK | MB_ICONSTOP
                );
                throw std::runtime_error("Unable to create UKCP Root");
            }
        }

        /*
            Get the My Documents path
        */
        std::wstring ExternalsBootstrap::GetMyDocumentsPath(void)
        {
            TCHAR myDocumentsPath[MAX_PATH];
            HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocumentsPath);

            std::wstring widePath(myDocumentsPath);
            std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
            return widePath;
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
