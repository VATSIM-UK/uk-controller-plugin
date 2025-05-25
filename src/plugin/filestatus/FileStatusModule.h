#pragma once

#include "bootstrap/PersistenceContainer.h"

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

} // namespace UKControllerPlugin::FileStatus