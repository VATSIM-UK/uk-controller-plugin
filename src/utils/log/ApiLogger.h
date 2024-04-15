#pragma once
#include "ApiLoggerInterface.h"

namespace UKControllerPluginUtils::Log {
    class ApiLogger : public ApiLoggerInterface
    {
        public:
        ApiLogger();
        ~ApiLogger() override;
        void Log(const std::string& type, const std::string& message) const override;
        void Log(const std::string& type, const std::string& message, const nlohmann::json& metadata) const override;
        void LogAsync(const std::string& type, const std::string& message) const override;
        void
        LogAsync(const std::string& type, const std::string& message, const nlohmann::json& metadata) const override;

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
} // namespace UKControllerPluginUtils::Log
