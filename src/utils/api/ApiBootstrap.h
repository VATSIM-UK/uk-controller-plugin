#pragma once
#include "api/ApiInterface.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Setting {
        class SettingRepository;
    } // namespace Setting
    namespace Curl {
        class CurlInterface;
    } // namespace Curl
} // namespace UKControllerPlugin

namespace UKControllerPluginUtils::Api {
    class ApiFactory;

    std::unique_ptr<UKControllerPlugin::Api::ApiInterface>
    Bootstrap(ApiFactory& factory, UKControllerPlugin::Curl::CurlInterface& curl);
} // namespace UKControllerPluginUtils::Api
