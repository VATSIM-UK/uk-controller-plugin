#include "CurlRequest.h"

using UKControllerPluginUtils::Http::HttpMethod;

namespace UKControllerPlugin::Curl {

    CurlRequest::CurlRequest(std::string uri, std::string method)
        : method(CheckMethod(std::move(method))), uri(std::move(uri))
    {
    }

    CurlRequest::CurlRequest(std::string uri, HttpMethod method) : method(method), uri(std::move(uri))
    {
    }

    void CurlRequest::AddHeader(const std::string& key, std::string value)
    {
        if (this->headers.count(key) != 0) {
            throw std::logic_error("Header already exists " + key);
        }

        this->headers[key] = std::move(value);
    }

    /*
        Get the body of the request.
    */
    auto CurlRequest::GetBody() const -> const char*
    {
        return this->body.c_str();
    }

    /*
        Get the method of the request.
    */
    auto CurlRequest::GetMethod() const -> const char*
    {
        return this->method.c_str();
    }

    /*
        Returns the request URI
    */
    auto CurlRequest::GetUri() const -> const char*
    {
        return this->uri.c_str();
    }

    /*
        Returns true if two requests are the same
    */
    auto CurlRequest::operator==(const CurlRequest& compare) const -> bool
    {
        return this->uri == compare.uri && this->method == compare.method && this->body == compare.body &&
               this->headers == compare.headers && this->maxRequestTime == compare.maxRequestTime;
    }

    auto CurlRequest::GetMaxRequestTime() const -> INT64
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
    void CurlRequest::SetBody(const std::string& body)
    {
        if (this->body != nobody) {
            throw std::logic_error("Body already set");
        }

        if (this->method == METHOD_GET || this->method == METHOD_DELETE) {
            throw std::logic_error("This type of request cannot have a body");
        }

        this->body = body;
    }

    /*
        Sets the method of the request based on the HTTP verb.
    */
    auto CurlRequest::CheckMethod(std::string method) -> std::string
    {
        if (method != METHOD_PUT && method != METHOD_GET && method != METHOD_POST && method != METHOD_DELETE &&
            method != METHOD_PATCH) {
            throw std::invalid_argument("Invalid HTTP method");
        }

        return method;
    }
} // namespace UKControllerPlugin::Curl
