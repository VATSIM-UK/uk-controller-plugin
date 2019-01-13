#pragma once
#include "api/ApiResponse.h"

// Forward Declarations
namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    };  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    };  // namespace Api
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Update {

        /*
            A class with all static methods, its purpose is to poll the API
            and check for plugin updates.
        */
        class PluginUpdateChecker
        {
            public:
                static int CheckForUpdates(
                    std::string pluginVersion,
                    UKControllerPlugin::Windows::WinApiInterface & winApi,
                    UKControllerPlugin::Api::ApiInterface & api
                );


                // We were unable to confirm with the API
                static const int statusUnknown = -1;

                // The return code, if the version of the plugin is acceptable
                static const int versionAllowed = 0;

                // The return code, if the version of the plugin is not supported
                static const int unsupportedVersion = 1;

            private:

                static int CheckApiResponse(
                    int apiResponse,
                    std::string pluginVersion,
                    UKControllerPlugin::Windows::WinApiInterface & winApi
                );
        };
    }  // namespace Update
}  // namespace UKControllerPlugin
