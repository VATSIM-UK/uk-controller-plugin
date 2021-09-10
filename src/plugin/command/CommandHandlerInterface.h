#pragma once

namespace UKControllerPlugin::Command {

    /*
        An interface for classes that are to handle Euroscope dot commands.
    */
    class CommandHandlerInterface
    {
        public:
        CommandHandlerInterface();
        virtual ~CommandHandlerInterface();
        CommandHandlerInterface(const CommandHandlerInterface&);
        CommandHandlerInterface(const CommandHandlerInterface&&) = delete;
        [[nodiscard]] auto operator=(const CommandHandlerInterface&) -> CommandHandlerInterface&;
        [[nodiscard]] auto operator=(const CommandHandlerInterface&&) -> CommandHandlerInterface& = delete;
        /*
            Processes a given string command from Euroscope. Should return true if the command
            has been processed, false otherwise.
        */
        virtual auto ProcessCommand(std::string command) -> bool = 0;
    };
} // namespace UKControllerPlugin::Command
