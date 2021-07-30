#pragma once
#include "api/ApiException.h"

namespace UKControllerPlugin {
    namespace Api {
        /*
            Custom exception for when the API returns 404
        */
        class ApiNotFoundException : public ApiException
        {
            public:
                explicit ApiNotFoundException(std::string message)
                    : ApiException("ApiNotFoundException: " + message) {}
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
