#pragma once

namespace UKControllerPlugin::Integration {
    class ExternalMessageHandlerInterface
    {
        public:
        ExternalMessageHandlerInterface();
        virtual ~ExternalMessageHandlerInterface();
        ExternalMessageHandlerInterface(const ExternalMessageHandlerInterface&);
        ExternalMessageHandlerInterface(ExternalMessageHandlerInterface&&) = delete;
        [[nodiscard]] auto operator=(const ExternalMessageHandlerInterface&) -> ExternalMessageHandlerInterface&;
        [[nodiscard]] auto operator=(ExternalMessageHandlerInterface&&) -> ExternalMessageHandlerInterface& = delete;

        /*
            Process a message, return true if processed (propogration should stop)
            or false otherwise.
        */
        virtual bool ProcessMessage(std::string message) = 0;
    };
} // namespace UKControllerPlugin::Integration
