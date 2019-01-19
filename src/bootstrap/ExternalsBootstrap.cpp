#include "pch/stdafx.h"
#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlApi.h"
#include "windows/WinApi.h"
#include "graphics/GdiplusBrushes.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "bootstrap/LocateApiSettings.h"
#include "euroscope/GeneralSettingsDialog.h"

using UKControllerPlugin::Curl::CurlApi;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Windows::WinApi;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Windows::GdiGraphicsWrapper;
using UKControllerPlugin::Euroscope::GeneralSettingsDialog;

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
            CWnd * test = CWnd::FromHandle(GetActiveWindow());
            persistence.windows.reset(
                new WinApi(
                    instance,
                    "ukcp",
                    L"ukcp",
                    GeneralSettingsDialog(CWnd::FromHandle(GetActiveWindow()), *persistence.pluginUserSettingHandler)
                )
            );
            persistence.brushes.reset(new GdiplusBrushes);
            persistence.graphics.reset(new GdiGraphicsWrapper);
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
