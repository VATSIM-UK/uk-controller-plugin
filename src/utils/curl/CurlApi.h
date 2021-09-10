#pragma once
#include "curl/CurlInterface.h"

namespace UKControllerPlugin::Curl {
    /*
        An API to the CURL library, for sending CURL requests to third parties.
    */
    class CurlApi : public CurlInterface
    {
        public:
        CurlApi();
        UKControllerPlugin::Curl::CurlResponse
        MakeCurlRequest(const UKControllerPlugin::Curl::CurlRequest& request) override;

        private:
        static auto WriteFunction(void* ptr, size_t size, size_t nmemb, void* notused) -> size_t;
        const std::string userAgent;
    };
} // namespace UKControllerPlugin::Curl
