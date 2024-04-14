#pragma once

namespace UKControllerPluginUtils::Log {
    /**
     * An interface for logging things to the API where we need more information
     * or just want to know what's going on.
     */
    class ApiLoggerInterface
    {
        public:
        virtual ~ApiLoggerInterface() = default;
        virtual void Log(const std::string& type, const std::string& message) const = 0;
        virtual void Log(const std::string& type, const std::string& message, const nlohmann::json& metadata) const = 0;
        virtual void LogAsync(const std::string& type, const std::string& message) const = 0;
        virtual void
        LogAsync(const std::string& type, const std::string& message, const nlohmann::json& metadata) const = 0;
    };
} // namespace UKControllerPluginUtils::Log
