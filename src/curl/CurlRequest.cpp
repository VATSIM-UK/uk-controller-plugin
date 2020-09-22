#include "pch/stdafx.h"
#include "curl/CurlRequest.h"

namespace UKControllerPlugin {
    namespace Curl {

        // Define the HTTP verbs
        const std::string CurlRequest::METHOD_DELETE = "DELETE";
        const std::string CurlRequest::METHOD_GET = "GET";
        const std::string CurlRequest::METHOD_POST = "POST";
        const std::string CurlRequest::METHOD_PUT = "PUT";
        const std::string CurlRequest::nobody = "";

        CurlRequest::CurlRequest(std::string uri, std::string method)
            : uri(uri), method(this->CheckMethod(method))
        {

        }

        void CurlRequest::AddHeader(std::string key, std::string value)
        {
            if (this->headers.count(key.c_str()) != 0) {
                throw std::logic_error("Header already exists " + key);
            }

            this->headers[key.c_str()] = value.c_str();
        }

        /*
            Get the body of the request.
        */
        const char * const CurlRequest::GetBody(void) const
        {
            return this->body.c_str();
        }

        /*
            Get the method of the request.
        */
        const char * const CurlRequest::GetMethod(void) const
        {
            return this->method.c_str();
        }

        /*
            Returns the request URI
        */
        const char * const CurlRequest::GetUri(void) const
        {
            return this->uri.c_str();
        }

        /*
            Returns true if two requests are the same
        */
        bool CurlRequest::operator==(const CurlRequest & compare) const
        {
            return this->uri == compare.uri &&
                this->method == compare.method &&
                this->body == compare.body &&
                this->headers == compare.headers &&
                this->maxRequestTime == compare.maxRequestTime;
        }

        INT64 CurlRequest::GetMaxRequestTime(void) const
        {
            return this->maxRequestTime;
        }

        void CurlRequest::SetMaxRequestTime(INT64 requestTime)
        {
            this->maxRequestTime = requestTime;
        }

        /*
            Sets the body of the request, if its valid
        */
        void CurlRequest::SetBody(std::string body)
        {
            if (this->body != this->nobody) {
                throw std::logic_error("Body already set");
            }

            if (this->method == this->METHOD_GET || this->method == this->METHOD_DELETE) {
                throw std::logic_error("This type of request cannot have a body");
            }

            this->body = body.c_str();
        }

        /*
            Sets the method of the request based on the HTTP verb.
        */
        std::string CurlRequest::CheckMethod(std::string method)
        {
            if (method != this->METHOD_PUT &&
                method != this->METHOD_GET &&
                method != this->METHOD_POST &&
                method != this->METHOD_DELETE
            ) {
                throw std::invalid_argument("Invalid HTTP method");
            }

            return method;
        }
    }  // namespace Curl
}  // namespace UKControllerPlugin
