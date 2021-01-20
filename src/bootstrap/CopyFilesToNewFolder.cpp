#include "pch/stdafx.h"
#include "bootstrap/CopyFilesToNewFolder.h"
#include "windows/WinApiInterface.h"
#include "bootstrap/ExternalsBootstrap.h"

namespace UKControllerPlugin {
    namespace Bootstrap {

        void CopyFilesToNewFolder(Windows::WinApiInterface & windows)
        {
            // If we're not migrating from the old folder, skip
            if (
                std::filesystem::exists(ExternalsBootstrap::GetPluginFileRoot()) ||
                !std::filesystem::exists(ExternalsBootstrap::GetLegacyPluginFileRoot())
            ) {
                return;
            }

            // Create the root folder
            ExternalsBootstrap::SetupUkcpFolderRoot(windows);


            // Copy the files and notify if it breaks
            try {
                std::filesystem::copy(
                    ExternalsBootstrap::GetLegacyPluginFileRoot(),
                    ExternalsBootstrap::GetPluginFileRoot(),
                    std::filesystem::copy_options::recursive
                );
            } catch (std::filesystem::filesystem_error e) {
                windows.OpenMessageBox(
                    L"Unable to copy UKCP files to new root, please contact the VATSIM UK Web Servics Department.",
                    L"UKCP Fatal Error",
                    MB_OK | MB_ICONSTOP
                );
                throw std::runtime_error("Unable to move files to new UKCP Root");
            }
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
