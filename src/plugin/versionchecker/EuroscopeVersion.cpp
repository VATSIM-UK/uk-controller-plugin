#include "EuroscopeVersion.h"
#include "log/LoggerFunctions.h"

namespace UKControllerPlugin::VersionChecker {

    constexpr const char* LATEST_VERSION = "3.2.3.2";
    const size_t PATH_BUFFER_SIZE = 256;

    auto EuroscopeVersion::GetCurrentVersion() -> std::string
    {
        std::vector<char> path(PATH_BUFFER_SIZE);

        if (!GetModuleFileNameA(nullptr, path.data(), PATH_BUFFER_SIZE)) {
            LogError("Failed to get module file name");
            return "";
        }

        auto versionInfoSize = GetFileVersionInfoSizeA(path.data(), nullptr);
        std::vector<uint8_t> versionInfo(versionInfoSize);

        void* versionString;
        unsigned int versionStringSize;

        if (GetFileVersionInfoA(path.data(), 0, versionInfoSize, versionInfo.data()) &&
            VerQueryValueA(
                versionInfo.data(), "\\StringFileInfo\\040904b0\\FileVersion", &versionString, &versionStringSize)) {
            std::string version(static_cast<const char*>(versionString), versionStringSize);
            std::erase(version, '\0');
            return version;
        } else {
            LogError("Failed to get file version");
            return "";
        }
    }

    auto EuroscopeVersion::GetLatestVersion() -> std::string
    {
        return LATEST_VERSION;
    }

    auto EuroscopeVersion::GetUpdateMessage() -> std::string
    {
        return "Your copy of EuroScope is of the wrong version; please download the correct version from "
               "<https://docs.vatsim.uk/es-setup>.";
    }

} // namespace UKControllerPlugin::VersionChecker
