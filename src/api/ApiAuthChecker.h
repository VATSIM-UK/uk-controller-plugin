#pragma once

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    }  // namespace Api
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
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
                    const UKControllerPlugin::Api::ApiInterface & api,
                    UKControllerPlugin::Windows::WinApiInterface & windows
                );
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
