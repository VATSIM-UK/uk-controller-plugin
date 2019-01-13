#include "pch/stdafx.h"
#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlApi.h"
#include "windows/WinApi.h"
#include "graphics/GdiplusBrushes.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "bootstrap/LocateApiSettings.h"

using UKControllerPlugin::Curl::CurlApi;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Windows::WinApi;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Windows::GdiGraphicsWrapper;
namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Bootstraps the external services and also some Gdiplus resources.
        */
        void ExternalsBootstrap::Bootstrap(PersistenceContainer & persistence, HINSTANCE instance)
        {
            persistence.curl.reset(new CurlApi());
            persistence.windows.reset(new WinApi(instance, "ukcp", L"ukcp"));
            persistence.brushes.reset(new GdiplusBrushes);
            persistence.graphics.reset(new GdiGraphicsWrapper);
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
