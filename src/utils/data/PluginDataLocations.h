#pragma once

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

std::wstring GetUpdaterBinaryRelativePath();
std::wstring GetCoreBinaryRelativePath();
std::wstring GetOldCoreBinaryRelativePath();
std::wstring GetOldUpdaterBinaryRelativePath();
std::wstring GetOldUpdaterBinaryRelativePath();
std::wstring GetOldFileExtension(const std::wstring& file);
std::wstring GetBinariesFolderRelativePath();
std::wstring GetFullPluginDataRoot();
void CreatePluginDataRoot(UKControllerPlugin::Windows::WinApiInterface& windows);
