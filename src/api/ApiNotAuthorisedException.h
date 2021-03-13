#pragma once
#include "api/ApiException.h"

namespace UKControllerPlugin {
    namespace Api {
        /*
            Custom exception for when the API returns Unauthorised or Forbidden
        */
        class ApiNotAuthorisedException : public ApiException
        {
            public:
                ApiNotAuthorisedException(std::string message)
                    : ApiException("ApiNotAuthorisedException: " + message) {}
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
