#pragma once

#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlInterface.h"
#include <string>

namespace UKControllerPlugin::Curl {
    class CurlInterface;
}

namespace UKControllerPlugin::FileStatus {

    /*
     * Module for checking file status during plugin initialization
     */
    class FileStatusModule
    {
        public:
        /*
         * Bootstrap the module into the plugin
         */
        static void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& container);
    };

    /*
     * Fetches the controller pack version from the docs.vatsim.uk
     */
    std::string FetchPackVersion(UKControllerPlugin::Curl::CurlInterface& curl);

    /*
     * Checks if the local controller pack version matches the one on GitHub
     */
    void CheckPackVersion(UKControllerPlugin::Bootstrap::PersistenceContainer& container);

    /*
     * Checks if the sector file provider configuration is up to date
     */
    void CheckSectorFileProviderFile(UKControllerPlugin::Bootstrap::PersistenceContainer& container);

} // namespace UKControllerPlugin::FileStatus