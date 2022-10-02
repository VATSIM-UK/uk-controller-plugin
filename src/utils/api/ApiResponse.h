#pragma once

namespace UKControllerPlugin {
    namespace Api {

        /*
            A class that holds the response from an API call.
        */
        class ApiResponse
        {
            public:
            ApiResponse(bool error, uint64_t statusCode, nlohmann::json rawData, std::string message);
            nlohmann::json GetRawData(void) const;
            std::string GetResponseMessage(void) const;
            uint64_t GetStatusCode(void) const;
            bool IsBadRequest(void) const;
            bool IsCurlError(void) const;
            bool IsNotFound(void) const;
            bool IsServerError(void) const;
            bool IsSuccess(void) const;
            bool IsTeapot(void) const;
            bool IsUnauthorised(void) const;

            // The HTTP status codes that may be returned by the API
            const uint64_t STATUS_OK = 200L;
            const uint64_t STATUS_CREATED = 201L;
            const uint64_t STATUS_NO_CONTENT = 204L;
            const uint64_t STATUS_BAD_REQUEST = 400L;
            const uint64_t STATUS_UNAUTHORISED = 401L;
            const uint64_t STATUS_FORBIDDEN = 403L;
            const uint64_t STATUS_NOT_FOUND = 404L;
            const uint64_t STATUS_TEAPOT = 418L;
            const uint64_t STATUS_SERVER_ERROR = 500L;
            const uint64_t STATUS_SERVICE_UNAVAILBLE = 503L;

            private:
            // Whether or not there was an error whilst making the API request.
            bool error;

            // The status code returned by the API
            uint64_t statusCode;

            // The message which may or may not be forwarded to the user.
            std::string message;

            // The raw JSON response.
            nlohmann::json rawResponse;
        };
    } // namespace Api
} // namespace UKControllerPlugin
