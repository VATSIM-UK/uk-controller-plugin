#pragma once


namespace UKControllerPlugin {
    namespace Curl {
        /*
            Custom exception for when something goes wrong with CURL.
        */
        class HttpException : public std::runtime_error
        {
            public:
                HttpException::HttpException(std::string message) : runtime_error("HttpException: " + message) {}
        };
    }  // namespace Curl
}  // namespace UKControllerPlugin
