#include "utils/pch.h"
#include "data/PluginDataLocations.h"
#include "windows/WinApiInterface.h"

std::wstring GetUpdaterBinaryRelativePath()
{
    return GetBinariesFolderRelativePath() + L"/UKControllerPluginUpdater.dll";
}

std::wstring GetCoreBinaryRelativePath()
{
    return GetBinariesFolderRelativePath() + L"/UKControllerPluginCore.dll";
}

std::wstring GetFullPluginDataRoot(void)
{
    TCHAR* folderPath = nullptr;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_SIMPLE_IDLIST, nullptr, &folderPath);

    std::wstring widePath(folderPath);
    std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
    CoTaskMemFree(folderPath);
    return widePath + L"/UKControllerPlugin";
}

std::wstring GetBinariesFolderRelativePath()
{
    return L"bin";
}

void CreatePluginDataRoot(UKControllerPlugin::Windows::WinApiInterface& windows)
{
    std::wstring rootPath = GetFullPluginDataRoot();
    if (!windows.CreateFolderRecursive(rootPath)) {
        windows.OpenMessageBox(
            L"Unable to create the UKCP root folder, please contact the VATUK Web Department.",
            L"UKCP Fatal Error",
            MB_OK | MB_ICONSTOP
        );
        throw std::runtime_error("Unable to create UKCP Root");
    }

    if (!windows.SetPermissions(rootPath, std::filesystem::perms::all)) {
        windows.OpenMessageBox(
            L"Unable to set permissions on the UKCP root folder, please contact the VATUK Web Department.",
            L"UKCP Fatal Error",
            MB_OK | MB_ICONSTOP
        );
        throw std::runtime_error("Unable to set permissions on the UKCP root folder");
    }
}
