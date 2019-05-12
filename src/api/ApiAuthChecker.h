#pragma once

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
        class ApiRequestBuilder;
    }  // namespace Api
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
    namespace Setting {
        class SettingRepository;
    }  // namespace Setting
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Api {

        /*
            A class whose purpose is to poll the API to check the users access
            token and display a message to the user if not. This is primarly
            used to plugin loadup so we don't have to check authorisation in other
            parts of the code.
        */
        class ApiAuthChecker
        {
            public:
                static bool IsAuthorised(
                    UKControllerPlugin::Api::ApiInterface & api,
                    UKControllerPlugin::Windows::WinApiInterface & windows,
                    UKControllerPlugin::Setting::SettingRepository & settings
                );
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
