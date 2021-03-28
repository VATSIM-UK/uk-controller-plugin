#pragma once

namespace UKControllerPlugin {
    namespace Setting {
        class SettingRepository;
    } // namespace Setting
    namespace Curl {
        class CurlInterface;
    }  // namespace Curl

    namespace Api {
        // Forward declare
        class ApiInterface;

        std::unique_ptr<ApiInterface> Bootstrap(
            const Setting::SettingRepository& settings,
            Curl::CurlInterface& curl
        );
    } // namespace Api
} // namespace UKControllerPlugin
