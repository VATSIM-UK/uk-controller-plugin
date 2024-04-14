#include "ApiLogger.h"
#include "api/ApiRequestFactory.h"
#include "api/ApiRequestException.h"
#include "update/PluginVersion.h"

namespace UKControllerPluginUtils::Log {

    struct ApiLogger::Impl
    {
        [[nodiscard]] auto
        CreatePayloadNoMetadata(const std::string& type, const std::string& message) const -> nlohmann::json
        {
            return {{"type", type}, {"message", message}, {"metadata", PluginVersionMetadata().dump()}};
        }

        [[nodiscard]] auto CreatePayload(
            const std::string& type, const std::string& message, const nlohmann::json& metadata) const -> nlohmann::json
        {
            auto metadataWithVersion = PluginVersionMetadata();
            metadataWithVersion.update(metadata);
            return {{"type", type}, {"message", message}, {"metadata", metadataWithVersion.dump()}};
        }

        [[nodiscard]] auto PluginVersionMetadata() const -> nlohmann::json
        {
            return {{"plugin_version", UKControllerPlugin::Plugin::PluginVersion::version}};
        }

        void WriteLog(const nlohmann::json& data)
        {
            ApiRequest()
                .Post("plugin/logs", data)
                .Then([](const UKControllerPluginUtils::Api::Response& response) {
                    const auto data = response.Data();
                    if (!data.is_object() || !data.contains("id") || !data["id"].is_string()) {
                        LogError("Failed to send log to API, response was not as expected");
                        return;
                    }

                    LogInfo("Log sent to API with ID " + data["id"].get<std::string>());
                })
                .Catch([](const Api::ApiRequestException& exception) {
                    LogError(
                        "Failed to send log to API, status code was " +
                        std::to_string(static_cast<uint64_t>(exception.StatusCode())));
                })
                .Await();
        }

        void WriteLogAsync(const nlohmann::json& data)
        {
            ApiRequest().Post("plugin/logs", data).Catch([](const Api::ApiRequestException& exception) {
                LogError(
                    "Failed to send log to API, status code was " +
                    std::to_string(static_cast<uint64_t>(exception.StatusCode())));
            });
        }
    };

    ApiLogger::ApiLogger() : impl(std::make_unique<Impl>())
    {
    }

    ApiLogger::~ApiLogger() = default;

    void ApiLogger::Log(const std::string& type, const std::string& message) const
    {
        impl->WriteLog(impl->CreatePayloadNoMetadata(type, message));
    }

    void ApiLogger::Log(const std::string& type, const std::string& message, const nlohmann::json& metadata) const
    {
        impl->WriteLog(impl->CreatePayload(type, message, metadata));
    }

    void ApiLogger::LogAsync(const std::string& type, const std::string& message) const
    {
        impl->WriteLogAsync(impl->CreatePayloadNoMetadata(type, message));
    }

    void ApiLogger::LogAsync(const std::string& type, const std::string& message, const nlohmann::json& metadata) const
    {
        impl->WriteLogAsync(impl->CreatePayload(type, message, metadata));
    }
} // namespace UKControllerPluginUtils::Log
