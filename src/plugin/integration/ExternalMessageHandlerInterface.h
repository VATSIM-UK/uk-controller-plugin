#pragma once

namespace UKControllerPlugin::Integration {
    /*
        Interfaces for classes that want to process messages from external sources.
    */
    class ExternalMessageHandlerInterface
    {
        public:
        /*
            Process a message, return true if processed (propogration should stop)
            or false otherwise.
        */
        [[nodiscard]] virtual auto ProcessMessage(std::string message) -> bool = 0;
    };
} // namespace UKControllerPlugin::Integration
