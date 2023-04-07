#pragma once
#include "api/ApiResponse.h"
#include "curl/CurlResponse.h"

namespace UKControllerPlugin {
    namespace Api {

        class ApiResponseFactory
        {
            public:
            static ApiResponse Create(UKControllerPlugin::Curl::CurlResponse curl);
        };
    } // namespace Api
} // namespace UKControllerPlugin
