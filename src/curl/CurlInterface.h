#pragma once
#include "curl/CurlResponse.h"

namespace UKControllerPlugin {
    namespace Curl {
        class CurlRequest;
    }  // namespace Curl
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Curl {
        // Pre Declarations
        class HttpException;
        // END

        /*
            Interface to the libcurl library.
        */
        class CurlInterface
        {
            public:
                virtual UKControllerPlugin::Curl::CurlResponse MakeCurlRequest(
                    const UKControllerPlugin::Curl::CurlRequest & request
                ) = 0;
                ~CurlInterface(void) {}  // namespace Curl

        };
    }  // namespace Curl
}  // namespace UKControllerPlugin
