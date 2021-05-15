#include "utils/pch.h"
#include "api/ApiResponse.h"


namespace UKControllerPlugin {
    namespace Api {

        ApiResponse::ApiResponse(bool error, uint64_t statusCode, nlohmann::json rawData, std::string message)
        {
            this->statusCode = statusCode;
            this->message = message;
            this->rawResponse = rawData;
            this->error = error;
        }

        /*
            Returns the raw data.
        */
        nlohmann::json ApiResponse::GetRawData(void) const
        {
            return this->rawResponse;
        }

        /*
            Returns the message.
        */
        std::string ApiResponse::GetResponseMessage(void) const
        {
            return this->message;
        }

        /*
            Returns the status code
        */
        uint64_t ApiResponse::GetStatusCode(void) const
        {
            return this->statusCode;
        }

        /*
            Returns whether or not the request was authorised.
        */
        bool ApiResponse::IsUnauthorised(void) const
        {
            return this->statusCode == this->STATUS_FORBIDDEN ||
                this->statusCode == this->STATUS_UNAUTHORISED;
        }

        bool ApiResponse::IsBadRequest(void) const
        {
            return this->statusCode == this->STATUS_BAD_REQUEST;
        }

        /*
            Returns whether or not the request caused a cURL error
        */
        bool ApiResponse::IsCurlError(void) const
        {
            return this->error;
        }

        bool ApiResponse::IsNotFound(void) const
        {
            return this->statusCode == this->STATUS_NOT_FOUND;
        }

        bool ApiResponse::IsServerError(void) const
        {
            return this->statusCode == this->STATUS_SERVER_ERROR ||
                this->statusCode == this->STATUS_SERVICE_UNAVAILBLE;
        }

        /*
            Returns the success status.
        */
        bool ApiResponse::IsSuccess(void) const
        {
            return this->statusCode == this->STATUS_OK ||
                this->statusCode == this->STATUS_CREATED ||
                this->statusCode == this->STATUS_NO_CONTENT;
        }

        bool ApiResponse::IsTeapot(void) const
        {
            return this->statusCode == this->STATUS_TEAPOT;
        }
    }  // namespace Api
}  // namespace UKControllerPlugin
