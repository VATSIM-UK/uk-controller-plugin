#pragma once
#include "command/CommandHandlerInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Hold {
    class HoldManager;

    /**
     * Handles the commands that can add aircraft to the hold manager.
     */
    class AssignHoldCommand : public Command::CommandHandlerInterface
    {
        public:
        AssignHoldCommand(HoldManager& holdManager, Euroscope::EuroscopePluginLoopbackInterface& plugin);
        auto ProcessCommand(std::string command) -> bool override;

        private:
        // The hold manager
        HoldManager& holdManager;
        
        // The plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The command regex for unassigning the hold
        std::regex noholdRegex;

        // The command for adding aircraft to a hold
        std::regex holdRegex;
    };
} // namespace UKControllerPlugin::Hold
