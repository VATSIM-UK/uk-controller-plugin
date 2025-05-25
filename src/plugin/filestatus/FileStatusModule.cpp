#include "FileStatusModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "windows/WinApiInterface.h"
#include "data/PluginDataLocations.h"
#include "log/LoggerFunctions.h"

#include <fstream>
#include <sstream>

namespace UKControllerPlugin::FileStatus {

    void FileStatusModule::BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& container)
    {
        // Attempt to read version file
        try {
            std::string versionFilePath = "./UK/Data/Plugin/version.txt"; // local
            std::ifstream versionFile(versionFilePath);

            // Probably a very old version (pre version file)
            if (!std::filesystem::exists(versionFilePath)) {
                LogError("Version file not found at: " + versionFilePath);
                container.windows->OpenMessageBox(
                    L"You must update your controller pack",
                    L"Out of Date Controller Pack Detected",
                    MB_OK | MB_ICONWARNING);
                return;
            }

            if (!versionFile.is_open()) {
                LogError("Could not open version.txt when checking CPa version");
            }

            std::stringstream buffer;
            buffer << versionFile.rdbuf();
            std::string versionContent = buffer.str();

            // TODO process the file
            LogInfo("Version.txt content: " + versionContent);

            versionFile.close();
        } catch (const std::exception& e) {
            LogError("Excepting when checking UKCPa version");
        }
    }

} // namespace UKControllerPlugin::FileStatus