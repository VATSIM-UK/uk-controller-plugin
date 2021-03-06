#pragma once
#include "windows/WinApiInterface.h"
#include "dialog/DialogProviderInterface.h"

namespace UKControllerPlugin {
    namespace Windows {

        /*
            A concrete implementation of the WinApiInterface. Provides a wrapper
            around certain WindowsFunctions such as opening dialog boxes and playing sounds.
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
                int OpenMessageBox(LPCWSTR message, LPCWSTR title, int options) override;
                void OpenWebBrowser(std::wstring url) override;
                void PlayWave(LPCTSTR sound);
                std::string ReadFromFile(std::wstring filename, bool relativePath = true) override;
                bool SetPermissions(std::wstring fileOrFolder, std::filesystem::perms permissions) override;
                void WriteToFile(std::wstring filename, std::string data, bool truncate) override;

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
