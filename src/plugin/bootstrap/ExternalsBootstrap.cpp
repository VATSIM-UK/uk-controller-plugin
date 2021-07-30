#include "pch/pch.h"
#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlApi.h"
#include "windows/WinApi.h"
#include "graphics/GdiplusBrushes.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "euroscope/GeneralSettingsDialog.h"
#include "dialog/DialogManager.h"

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
            std::unique_ptr<WinApi> winApi = std::make_unique<WinApi>(
                instance,
                GetPluginFileRoot()
            );
            persistence.dialogManager.reset(new DialogManager(*winApi));
            persistence.windows = std::move(winApi);
            persistence.brushes.reset(new GdiplusBrushes);
            persistence.graphics.reset(new GdiGraphicsWrapper);
        }

        /*
            Return the path to UKCP local files in wide format. This is located
            in the Windows Local App Data known folder.
        */
        std::wstring ExternalsBootstrap::GetPluginFileRoot(void)
        {
            return GetLocalAppDataPath() + L"/UKControllerPlugin";
        }

        /*
            Return the path to UKCP local files in wide format. This is the LEGACY
            folder and should not be used going forward.
        */
        std::wstring ExternalsBootstrap::GetLegacyPluginFileRoot(void)
        {
            return GetMyDocumentsPath() + L"/EuroScope/ukcp";
        }

        /*
            Create the required folder hierarchy for UKCP.
        */
        void ExternalsBootstrap::SetupUkcpFolderRoot(WinApiInterface & winApi)
        {
            std::wstring rootPath = GetPluginFileRoot();
            if (!winApi.CreateFolderRecursive(rootPath)) {
                winApi.OpenMessageBox(
                    L"Unable to create the UKCP root folder, please contact the VATUK Web Department.",
                    L"UKCP Fatal Error",
                    MB_OK | MB_ICONSTOP
                );
                throw std::runtime_error("Unable to create UKCP Root");
            }

            if (!winApi.SetPermissions(rootPath, std::filesystem::perms::all)) {
                winApi.OpenMessageBox(
                    L"Unable to set permissions on the UKCP root folder, please contact the VATUK Web Department.",
                    L"UKCP Fatal Error",
                    MB_OK | MB_ICONSTOP
                );
                throw std::runtime_error("Unable to set permissions on the UKCP root folder");
            }
        }

        /*
         * Get the My Documents path
         */
        std::wstring ExternalsBootstrap::GetMyDocumentsPath(void)
        {
            return GetKnownFolderPath(FOLDERID_Documents);
        }

        /*
         * Get the local app data path.
         */
        std::wstring ExternalsBootstrap::GetLocalAppDataPath()
        {
            return GetKnownFolderPath(FOLDERID_LocalAppData);
        }

        std::wstring ExternalsBootstrap::GetKnownFolderPath(GUID folderId)
        {
            TCHAR * folderPath = 0;
            HRESULT result = SHGetKnownFolderPath(folderId, KF_FLAG_SIMPLE_IDLIST, NULL, &folderPath);

            std::wstring widePath(folderPath);
            std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
            CoTaskMemFree(folderPath);
            return widePath;
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
