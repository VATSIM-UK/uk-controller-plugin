#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Windows {
        /*
            Interface between the plugin and the Windows API.
        */
        class WinApiInterface
        {
        public:
            explicit WinApiInterface(HINSTANCE dllInstance)
                : dllInstance(dllInstance)
            {
            };
            virtual ~WinApiInterface(void) {}  // namespace Windows
            virtual bool CreateFolder(std::string folder) = 0;
            virtual bool DeleteGivenFile(std::string filename) = 0;
            virtual bool FileExists(std::string filename) = 0;
            virtual std::wstring FileOpenDialog(
                std::wstring title,
                UINT numFileTypes,
                const COMDLG_FILTERSPEC * fileTypes
            ) const = 0;
            HINSTANCE GetDllInstance(void) const {
                return this->dllInstance;
            }
            virtual std::string GetFullPathToLocalFile(std::string relativePath) const = 0;
            virtual bool OpenDialog(int dialogId, DLGPROC callback = NULL, LPARAM params = NULL) const = 0;
            virtual void OpenGeneralSettingsDialog() = 0;
            virtual int OpenMessageBox(LPCWSTR message, LPCWSTR title, int options) = 0;
            virtual void PlayWave(LPCTSTR sound) = 0;
            virtual std::string ReadFromFile(std::string filename, bool relativePath = true) = 0;
            virtual std::string ReadFromFile(std::wstring filename, bool relativePath = true) = 0;
            virtual void WriteToFile(std::string filename, std::string data, bool truncate) = 0;

        private:
            // DLL Instance for the plugin
            const HINSTANCE dllInstance;
        };
    }  // namespace Windows
}  // namespace UKControllerPlugin
