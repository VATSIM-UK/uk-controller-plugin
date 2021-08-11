#pragma once
#include "api/ApiException.h"

namespace UKControllerPlugin::Api {
    /*
        Custom exception for when the API returns Unauthorised or Forbidden
    */
    class ApiNotAuthorisedException : public ApiException
    {
        public:
            explicit ApiNotAuthorisedException(std::string message);
    };
}  // namespace UKControllerPlugin::Api
