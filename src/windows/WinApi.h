#pragma once
#include "windows/WinApiInterface.h"
#include "dialog/DialogProviderInterface.h"
#include "hold/HoldConfigurationDialog.h"

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
                    std::string filesDirectory,
                    std::wstring filesDirectoryW
                );
                bool CreateFolder(std::string folder);
                bool CreateLocalFolderRecursive(std::string folder) override;
                bool DeleteGivenFile(std::string filename);
                bool FileExists(std::string filename);
                std::string GetFullPathToLocalFile(std::string relativePath) const;
                std::wstring GetFullPathToLocalFile(std::wstring relativePath) const;
                bool OpenDialog(int dialogId, DLGPROC callback = NULL, LPARAM params = NULL) const;
                std::wstring FileOpenDialog(
                    std::wstring title,
                    UINT numFileTypes,
                    const COMDLG_FILTERSPEC * fileTypes
                ) const override;
                int OpenMessageBox(LPCWSTR message, LPCWSTR title, int options) override;
                void PlayWave(LPCTSTR sound);
                std::string ReadFromFile(std::string filename, bool relativePath = true);
                std::string ReadFromFile(std::wstring filename, bool relativePath = true);
                void WriteToFile(std::string filename, std::string data, bool truncate);

                // Inherited via DialogProviderInterface
                void OpenDialog(
                    const UKControllerPlugin::Dialog::DialogData & dialog,
                    const UKControllerPlugin::Dialog::DialogCallArgument * argument
                ) const override;

            private:
                void CreateMissingDirectories(std::string endFile);
                std::string ReadFileContents(std::ifstream file);

                // DLL Instance for the plugin
                HINSTANCE dllInstance;

                // Working directory for file operations
                const std::string filesDirectory;

                // Working directory for file operations in widestring
                const std::wstring filesDirectoryW;
        };
    }  // namespace Windows
}  // namespace UKControllerPlugin
