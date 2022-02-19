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
        NotFound = 404L,
        MethodNotAllowed = 405L,
        Unprocessable = 422L,
        ServerError = 500L,
        BadGateway = 502L,
    };

    inline auto operator==(uint64_t first, HttpStatusCode second) -> bool
    {
        return first == static_cast<uint64_t>(second);
    }

    inline auto IsSuccessful(HttpStatusCode status) -> bool
    {
        return status == HttpStatusCode::Ok || status == HttpStatusCode::Created || status == HttpStatusCode::NoContent;
    }

    inline auto IsAuthenticationError(HttpStatusCode status) -> bool
    {
        return status == HttpStatusCode::Unauthorised || status == HttpStatusCode::Forbidden;
    }

    inline auto IsServerError(HttpStatusCode status) -> bool
    {
        return status == HttpStatusCode::ServerError || status == HttpStatusCode::BadGateway ||
               status == HttpStatusCode::Unknown;
    }
} // namespace UKControllerPluginUtils::Http
