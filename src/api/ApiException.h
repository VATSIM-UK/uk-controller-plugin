#pragma once


namespace UKControllerPlugin {
    namespace Api {
        /*
            Custom exception for when something goes wrong when
            communicating with the API.
        */
        class ApiException : public std::runtime_error
        {
            public:
                ApiException::ApiException(std::string message) : runtime_error(message) {}
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
