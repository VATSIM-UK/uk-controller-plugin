#pragma once

namespace UKControllerPlugin {
    namespace Curl {


        /*
            Object returned by the CurlApi.
        */
        class CurlResponse {

            public:
                CurlResponse(std::string response, bool curlError, uint64_t statusCode);
                std::string GetResponse(void) const;
                uint64_t GetStatusCode(void) const;
                bool IsCurlError(void) const;
                bool StatusOk(void) const;

            private:

                // The response text
                std::string response;

                // The HTTP status code
                uint64_t statusCode;

                // Whether or not there was an error in cURL.
                bool curlError;

                // Ok
                const uint64_t okStatus = 200;

                // Created
                const uint64_t createdStatus = 201;

                // No response body
                const uint64_t noBodyStatus = 204;
        };
    }  // namespace Curl
}  // namespace UKControllerPlugin
