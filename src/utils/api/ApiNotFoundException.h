#pragma once
#include "api/ApiException.h"

namespace UKControllerPlugin::Api {
    /*
        Custom exception for when the API returns 404
    */
    class ApiNotFoundException : public ApiException
    {
        public:
        explicit ApiNotFoundException(const std::string& message) : ApiException("ApiNotFoundException: " + message)
        {
        }
    };
} // namespace UKControllerPlugin::Api
