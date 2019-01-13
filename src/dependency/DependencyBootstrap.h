#pragma once
#include "dependency/DependencyCache.h"

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
    namespace Curl {
        class CurlInterface;
    }  // namespace Curl
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    }  // namespace Api
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Bootstrap {

        class DependencyBootstrap
        {
            public:
                static UKControllerPlugin::Dependency::DependencyCache Bootstrap(
                    UKControllerPlugin::Api::ApiInterface & api,
                    UKControllerPlugin::Windows::WinApiInterface & windows,
                    UKControllerPlugin::Curl::CurlInterface & curl
                );

            private:

                // Path to the manifest file
                static const std::string manifestFile;
        };
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
