#pragma once

namespace ECFMP::Plugin {
    class Sdk;
} // namespace ECFMP::Plugin

namespace UKControllerPlugin::Curl {
    class CurlInterface;
} // namespace UKControllerPlugin::Curl

namespace UKControllerPlugin::ECFMP {

    /**
     * Bootstraps the ECFMP module.
     */
    class ECFMPModuleFactory
    {
        public:
        ECFMPModuleFactory();
        ~ECFMPModuleFactory();
        [[nodiscard]] auto Sdk(Curl::CurlInterface& curl) -> std::shared_ptr<::ECFMP::Plugin::Sdk>;

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };

} // namespace UKControllerPlugin::ECFMP
