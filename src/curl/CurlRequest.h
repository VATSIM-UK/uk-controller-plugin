#pragma once

namespace UKControllerPlugin {
    namespace Curl {

        /*
            Class representing a curl request to the interwebs.
        */
        class CurlRequest
        {

            public:

                CurlRequest(std::string uri, std::string method);
                void AddHeader(std::string key, std::string value);
                const char * const GetBody(void) const;
                const char * const GetMethod(void) const;
                const char * const GetUri(void) const;
                bool operator==(const CurlRequest & compare) const;
                void SetBody(std::string body);

                // No request body
                static const std::string nobody;

                // HTTP Verbs
                static const std::string METHOD_GET;
                static const std::string METHOD_POST;
                static const std::string METHOD_PUT;
                static const std::string METHOD_DELETE;

                typedef std::map<std::string, std::string> HttpHeaders;
                typedef HttpHeaders::const_iterator const_iterator;
                const_iterator cbegin() const { return this->headers.cbegin(); }
                const_iterator cend() const { return this->headers.cend(); }

            private:

                std::string CheckMethod(std::string method);

                // One of the HTTP verbs
                std::string method;

                // The URL to hit
                std::string uri;

                // The body of the request
                std::string body = "";

                // Headers
                std::map<std::string, std::string> headers;
        };
    }  // namespace Curl
}  // namespace UKControllerPlugin
