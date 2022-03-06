#include "HttpMethod.h"

namespace UKControllerPluginUtils::Http {

    HttpMethod::HttpMethod(std::string method) : method(method)
    {
    }

    auto HttpMethod::Get() -> HttpMethod
    {
        return HttpMethod("GET");
    }

    auto HttpMethod::Post() -> HttpMethod
    {
        return HttpMethod("POST");
    }

    auto HttpMethod::Put() -> HttpMethod
    {
        return HttpMethod("PUT");
    }

    auto HttpMethod::Patch() -> HttpMethod
    {
        return HttpMethod("PATCH");
    }

    auto HttpMethod::Delete() -> HttpMethod
    {
        return HttpMethod("DELETE");
    }

    HttpMethod::operator std::string() const
    {
        return method;
    }

    auto HttpMethod::operator==(const HttpMethod& compare) const -> bool
    {
        return method == compare.method;
    }
} // namespace UKControllerPluginUtils::Http
