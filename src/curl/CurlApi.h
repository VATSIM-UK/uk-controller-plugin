#pragma once
#include "curl/CurlInterface.h"

namespace UKControllerPlugin {
    namespace Curl {
        /*
            An API to the CURL library, for sending CURL requests to third parties.
        */
        class CurlApi : public CurlInterface
        {
            public:
                UKControllerPlugin::Curl::CurlResponse MakeCurlRequest(
                    const UKControllerPlugin::Curl::CurlRequest & request
                );

            private:
                static size_t WriteFunction(void *ptr, size_t size, size_t nmemb, void * notused);
            };
    }  // namespace Curl
}  // namespace UKControllerPlugin
