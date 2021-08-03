#pragma once
#include "pch/pch.h"

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
            virtual ~WinApiInterface() = default; // namespace Windows
            virtual bool CreateFolder(std::wstring folder) = 0;
            virtual bool CreateFolderRecursive(std::wstring folder) = 0;
            virtual bool CreateLocalFolderRecursive(std::wstring folder) = 0;
            virtual bool DeleteGivenFile(std::wstring filename) = 0;
            virtual bool FileExists(std::wstring filename) = 0;
            virtual std::wstring FileOpenDialog(
                std::wstring title,
                UINT numFileTypes,
                const COMDLG_FILTERSPEC * fileTypes
            ) const = 0;
            HINSTANCE GetDllInstance(void) const {
                return this->dllInstance;
            }
            virtual std::wstring GetFullPathToLocalFile(std::wstring relativePath) const = 0;
            virtual std::set<std::wstring> ListAllFilenamesInDirectory(
                std::wstring relativePath
            ) const = 0;
            virtual HINSTANCE LoadLibraryRelative(std::wstring relativePath) const = 0;
            virtual FARPROC GetFunctionPointerFromLibrary(HINSTANCE libraryHandle, std::string functionName) const = 0;
            virtual bool MoveFileToNewLocation(std::wstring oldName, std::wstring newName) = 0;
            virtual void UnloadLibrary(HINSTANCE handle) const = 0;
            virtual int OpenMessageBox(LPCWSTR message, LPCWSTR title, int options) = 0;
            virtual void OpenWebBrowser(std::wstring url) = 0;
            virtual void PlayWave(LPCTSTR sound) = 0;
            virtual std::string ReadFromFile(std::wstring filename, bool relativePath = true) = 0;
            virtual bool SetPermissions(std::wstring fileOrFolder, std::filesystem::perms permissions) = 0;
            virtual void WriteToFile(std::wstring filename, std::string data, bool truncate, bool binary) = 0;

        private:
            // DLL Instance for the plugin
            const HINSTANCE dllInstance;
        };
    }  // namespace Windows
}  // namespace UKControllerPlugin
