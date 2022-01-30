#pragma once

namespace UKControllerPluginUtils::Http {
    enum class HttpStatusCode : uint64_t
    {
        Unknown = 0L,
        Ok = 200L,
        Created = 201L,
        NoContent = 204L,
        BadRequest = 400L,
        Unauthorised = 401L,
        Forbidden = 403L,
        ServerError = 500L,
        BadGateway = 502L,
    };

    inline auto operator==(uint64_t first, HttpStatusCode second) -> bool
    {
        return first == static_cast<uint64_t>(second);
    }
} // namespace UKControllerPluginUtils::Http
