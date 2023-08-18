#pragma once

namespace ECFMP::Plugin {
    class Sdk;
} // namespace ECFMP::Plugin

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Curl {
        class CurlInterface;
    } // namespace Curl
} // namespace UKControllerPlugin

namespace UKControllerPlugin::ECFMP {

    /**
     * Bootstraps the ECFMP module.
     */
    class ECFMPModuleFactory
    {
        public:
        ECFMPModuleFactory();
        ~ECFMPModuleFactory();
        [[nodiscard]] auto Sdk(Curl::CurlInterface& curl, const Controller::ActiveCallsignCollection& callsigns)
            -> std::shared_ptr<::ECFMP::Plugin::Sdk>;

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };

} // namespace UKControllerPlugin::ECFMP
