#pragma once
#include "windows/WinApiInterface.h"
#include "dialog/DialogProviderInterface.h"

namespace UKControllerPlugin {
    namespace Windows {

        /*
            A concrete implementation of the WinApiInterface. Provides a wrapper
            around certain WindowsFunctions such as opening dialog boxes and playing sounds.

            This class is restricted purely to operations within the UKCP root folder, to prevent
            spurious activity across the filesystem...
        */
        class WinApi : public WinApiInterface, public UKControllerPlugin::Dialog::DialogProviderInterface
        {
            public:
                WinApi(
                    HINSTANCE dllInstance,
                    std::wstring filesDirectory
                );
                bool CreateFolder(std::wstring folder) override;
                bool CreateFolderRecursive(std::wstring folder) override;
                bool CreateLocalFolderRecursive(std::wstring folder) override;
                bool DeleteGivenFile(std::wstring filename) override;
                bool FileExists(std::wstring filename) override;
                std::wstring GetFullPathToLocalFile(std::wstring relativePath) const override;
                std::wstring FileOpenDialog(
                    std::wstring title,
                    UINT numFileTypes,
                    const COMDLG_FILTERSPEC * fileTypes
                ) const override;
                std::set<std::wstring> ListAllFilenamesInDirectory(
                    std::wstring relativePath
                ) const override;
                HINSTANCE LoadLibraryRelative(std::wstring relativePath) const override;
                FARPROC GetFunctionPointerFromLibrary(HINSTANCE libraryHandle, std::string functionName) const override;
                void UnloadLibrary(HINSTANCE handle) const override;
                int OpenMessageBox(LPCWSTR message, LPCWSTR title, int options) override;
                void OpenWebBrowser(std::wstring url) override;
                void PlayWave(LPCTSTR sound) override;
                bool MoveFileToNewLocation(std::wstring oldName, std::wstring newName) override;
                std::string ReadFromFile(std::wstring filename, bool relativePath = true) override;
                bool SetPermissions(std::wstring fileOrFolder, std::filesystem::perms permissions) override;
                void WriteToFile(std::wstring filename, std::string data, bool truncate, bool binary) override;

                // Inherited via DialogProviderInterface
                void OpenDialog(
                    const UKControllerPlugin::Dialog::DialogData & dialog,
                    const UKControllerPlugin::Dialog::DialogCallArgument * argument
                ) const override;

            private:
                void CreateMissingDirectories(std::wstring endFile);
                std::string ReadFileContents(std::ifstream file);

                // DLL Instance for the plugin
                HINSTANCE dllInstance;

                // Working directory for file operations in widestring
                const std::wstring filesDirectory;
        };
    }  // namespace Windows
}  // namespace UKControllerPlugin
