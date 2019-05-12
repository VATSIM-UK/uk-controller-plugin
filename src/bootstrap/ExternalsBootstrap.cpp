#include "pch/stdafx.h"
#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlApi.h"
#include "windows/WinApi.h"
#include "graphics/GdiplusBrushes.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "bootstrap/LocateApiSettings.h"
#include "euroscope/GeneralSettingsDialog.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Curl::CurlApi;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Windows::WinApi;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Windows::GdiGraphicsWrapper;
using UKControllerPlugin::Euroscope::GeneralSettingsDialog;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Bootstraps the external services and also some Gdiplus resources.
        */
        void ExternalsBootstrap::Bootstrap(PersistenceContainer & persistence, HINSTANCE instance)
        {
            // Required so we can hit the dialog resource.
            AFX_MANAGE_STATE(AfxGetStaticModuleState());

            persistence.curl.reset(new CurlApi());

            // All files should be relative to Documents/EuroScope
            persistence.windows.reset(
                new WinApi(
                    instance,
                    ExternalsBootstrap::GetPluginFileRoot(),
                    ExternalsBootstrap::GetPluginFileRootWide(),
                    GeneralSettingsDialog(
                        CWnd::FromHandle(GetActiveWindow()),
                        *persistence.pluginUserSettingHandler,
                        *persistence.userSettingHandlers
                    )
                )
            );
            persistence.brushes.reset(new GdiplusBrushes);
            persistence.graphics.reset(new GdiGraphicsWrapper);
            persistence.windows->WriteToFile("test/test.json", "test", true);
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
            TCHAR myDocumentsPath[MAX_PATH];
            HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocumentsPath);


            std::wstring widePath(myDocumentsPath);
            std::replace(widePath.begin(), widePath.end(), L'\\', L'/');

            return widePath + L"/EuroScope/ukcp";
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
