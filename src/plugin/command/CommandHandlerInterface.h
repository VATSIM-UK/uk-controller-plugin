#pragma once

namespace UKControllerPlugin {
    namespace Command {

        /*
            An interface for classes that are to handle Euroscope dot commands.
        */
        class CommandHandlerInterface
        {
            public:

                /*
                    Processes a given string command from Euroscope. Should return true if the command
                    has been processed, false otherwise.
                */
                virtual bool ProcessCommand(std::string command) = 0;
        };
    }  // namespace Command
}  // namespace UKControllerPlugin
