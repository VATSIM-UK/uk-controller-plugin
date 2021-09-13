#include "ExternalsBootstrap.h"
#include "PersistenceContainer.h"
#include "curl/CurlApi.h"
#include "dialog/DialogManager.h"
#include "euroscope/GeneralSettingsDialog.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "graphics/GdiplusBrushes.h"
#include "windows/WinApi.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Curl::CurlApi;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::GeneralSettingsDialog;
using UKControllerPlugin::Windows::GdiGraphicsWrapper;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Windows::WinApi;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin::Bootstrap {

    /*
        Bootstraps the external services and also some Gdiplus resources.
    */
    void ExternalsBootstrap::Bootstrap(PersistenceContainer& persistence, HINSTANCE instance)
    {
        persistence.curl = std::move(std::make_unique<CurlApi>());
        std::unique_ptr<WinApi> winApi = std::make_unique<WinApi>(instance, GetPluginFileRoot());
        persistence.dialogManager = std::make_unique<DialogManager>(*winApi);
        persistence.windows = std::move(winApi);
        persistence.brushes = std::make_unique<GdiplusBrushes>();
        persistence.graphics = std::make_unique<GdiGraphicsWrapper>();
    }

    /*
        Return the path to UKCP local files in wide format. This is located
        in the Windows Local App Data known folder.
    */
    auto ExternalsBootstrap::GetPluginFileRoot() -> std::wstring
    {
        return GetLocalAppDataPath() + L"/UKControllerPlugin";
    }

    /*
        Return the path to UKCP local files in wide format. This is the LEGACY
        folder and should not be used going forward.
    */
    auto ExternalsBootstrap::GetLegacyPluginFileRoot() -> std::wstring
    {
        return GetMyDocumentsPath() + L"/EuroScope/ukcp";
    }

    /*
        Create the required folder hierarchy for UKCP.
    */
    void ExternalsBootstrap::SetupUkcpFolderRoot(WinApiInterface& winApi)
    {
        std::wstring rootPath = GetPluginFileRoot();
        if (!winApi.CreateFolderRecursive(rootPath)) {
            winApi.OpenMessageBox(
                L"Unable to create the UKCP root folder, please contact the VATUK Web Department.",
                L"UKCP Fatal Error",
                MB_OK | MB_ICONSTOP);
            throw std::runtime_error("Unable to create UKCP Root");
        }

        if (!winApi.SetPermissions(rootPath, std::filesystem::perms::all)) {
            winApi.OpenMessageBox(
                L"Unable to set permissions on the UKCP root folder, please contact the VATUK Web Department.",
                L"UKCP Fatal Error",
                MB_OK | MB_ICONSTOP);
            throw std::runtime_error("Unable to set permissions on the UKCP root folder");
        }
    }

    /*
     * Get the My Documents path
     */
    auto ExternalsBootstrap::GetMyDocumentsPath() -> std::wstring
    {
        return GetKnownFolderPath(FOLDERID_Documents);
    }

    /*
     * Get the local app data path.
     */
    auto ExternalsBootstrap::GetLocalAppDataPath() -> std::wstring
    {
        return GetKnownFolderPath(FOLDERID_LocalAppData);
    }

    auto ExternalsBootstrap::GetKnownFolderPath(GUID folderId) -> std::wstring
    {
        TCHAR* folderPath = nullptr;
        SHGetKnownFolderPath(folderId, KF_FLAG_SIMPLE_IDLIST, nullptr, &folderPath);

        std::wstring widePath(folderPath);
        std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
        CoTaskMemFree(folderPath);
        return widePath;
    }
} // namespace UKControllerPlugin::Bootstrap
