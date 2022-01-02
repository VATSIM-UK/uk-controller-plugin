#pragma once
#include "command/CommandHandlerInterface.h"

namespace UKControllerPlugin::Message {
    class UserMessager;
} // namespace UKControllerPlugin::Message

namespace UKControllerPlugin::Metar {
    class ParsedMetarCollection;

    class PressureQueryCommandHandler : public Command::CommandHandlerInterface
    {
        public:
        PressureQueryCommandHandler(const ParsedMetarCollection& metars, Message::UserMessager& userMessager);
        auto ProcessCommand(std::string command) -> bool override;

        private:
        // All the metars
        const ParsedMetarCollection& metars;

        // Sends messages to the user
        Message::UserMessager& userMessager;

        const std::regex commandPattern;
    };
} // namespace UKControllerPlugin::Metar
