#pragma once

namespace UKControllerPlugin {

    namespace Api {
        class ApiInterface;
    } // namespace Api

    namespace Windows {
        class WinApiInterface;
    } // namespace Windows

    namespace Curl {
        class CurlInterface;
        class CurlRequest;
    } // namespace Curl

    bool UpdateBinaries(
        nlohmann::json updateData,
        Windows::WinApiInterface& windows,
        Curl::CurlInterface& curl
    );

    nlohmann::json GetUpdateData(
        const Api::ApiInterface& api
    );

    bool UpdateBinary(
        Curl::CurlInterface& curl,
        Curl::CurlRequest& request,
        Windows::WinApiInterface& windows,
        std::wstring targetFile
    );

    bool UpdateDataValid(const nlohmann::json& updateData);
} // namespace UKControllerPlugin
