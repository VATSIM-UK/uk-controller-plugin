#include "PluginDataLocations.h"
#include "windows/WinApiInterface.h"

auto GetUpdaterBinaryRelativePath() -> std::wstring
{
    return GetBinariesFolderRelativePath() + L"/UKControllerPluginUpdater.dll";
}

auto GetCoreBinaryRelativePath() -> std::wstring
{
    return GetBinariesFolderRelativePath() + L"/UKControllerPluginCore.dll";
}

auto GetOldCoreBinaryRelativePath() -> std::wstring
{
    return GetOldFileExtension(GetCoreBinaryRelativePath());
}

auto GetOldUpdaterBinaryRelativePath() -> std::wstring
{
    return GetOldFileExtension(GetUpdaterBinaryRelativePath());
}

auto GetFullPluginDataRoot() -> std::wstring
{
    TCHAR* folderPath = nullptr;
    SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_SIMPLE_IDLIST, nullptr, &folderPath);

    std::wstring widePath(folderPath);
    std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
    CoTaskMemFree(folderPath);
    return widePath + L"/UKControllerPlugin";
}

auto GetBinariesFolderRelativePath() -> std::wstring
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
            MB_OK | MB_ICONSTOP);
        throw std::runtime_error("Unable to create UKCP Root");
    }

    if (!windows.SetPermissions(rootPath, std::filesystem::perms::all)) {
        windows.OpenMessageBox(
            L"Unable to set permissions on the UKCP root folder, please contact the VATUK Web Department.",
            L"UKCP Fatal Error",
            MB_OK | MB_ICONSTOP);
        throw std::runtime_error("Unable to set permissions on the UKCP root folder");
    }
}

std::wstring GetOldFileExtension(const std::wstring& file)
{
    return file + L".old";
}
