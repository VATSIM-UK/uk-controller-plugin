#pragma once

namespace UKControllerPlugin::ECFMP {

    /**
     * A logger instance that can be passed to ECFMP.
     */
    class Logger : public ::ECFMP::Log::Logger
    {
        public:
        void Debug(const std::string& message) override;
        void Info(const std::string& message) override;
        void Warning(const std::string& message) override;
        void Error(const std::string& message) override;
    };

} // namespace UKControllerPlugin::ECFMP
