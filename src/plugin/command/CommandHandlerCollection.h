#pragma once

// Forward declare
namespace UKControllerPlugin {
    namespace Command {
        class CommandHandlerInterface;
    } // namespace Command
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Command {

        /*
            A class that comprises one or more command handlers and delegates
            the processing of Euroscope dot commands to them.
        */
        class CommandHandlerCollection
        {
            public:
            size_t CountHandlers(void) const;
            void RegisterHandler(std::shared_ptr<UKControllerPlugin::Command::CommandHandlerInterface> handler);
            bool ProcessCommand(std::string command) const;

            private:
            // An array of all the handlers.
            std::vector<std::shared_ptr<UKControllerPlugin::Command::CommandHandlerInterface>> handlers;
        };
    } // namespace Command
} // namespace UKControllerPlugin
