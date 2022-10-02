#include "curl/CurlResponse.h"

namespace UKControllerPlugin {
    namespace Curl {

        CurlResponse::CurlResponse(std::string response, bool curlError, uint64_t statusCode)
        {
            this->response = response;
            this->statusCode = statusCode;
            this->curlError = curlError;
        }

        /*
            Returns the response.
        */
        std::string CurlResponse::GetResponse(void) const
        {
            return this->response;
        }

        /*
            Returns the status code.
        */
        uint64_t CurlResponse::GetStatusCode(void) const
        {
            return this->statusCode;
        }

        /*
            Returns true if there's a curl error.
        */
        bool CurlResponse::IsCurlError(void) const
        {
            return this->curlError;
        }

        /*
            Returns true if the status code is 200.
        */
        bool CurlResponse::StatusOk(void) const
        {
            return this->statusCode == this->okStatus || this->statusCode == this->createdStatus ||
                   this->statusCode == this->noBodyStatus;
        }
    } // namespace Curl
} // namespace UKControllerPlugin
