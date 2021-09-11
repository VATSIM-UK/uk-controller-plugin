#pragma once
#include "RadarRenderableCollection.h"
#include "command/CommandHandlerInterface.h"

namespace UKControllerPlugin::RadarScreen {

    /*
        Processes commands to reset the position of renderables
    */
    class PositionResetCommand : public UKControllerPlugin::Command::CommandHandlerInterface
    {
        public:
        explicit PositionResetCommand(const UKControllerPlugin::RadarScreen::RadarRenderableCollection& renderables);

        // Inherited via CommandHandlerInterface
        [[nodiscard]] auto ProcessCommand(std::string command) -> bool override;

        private:
        // Radar renderables
        const UKControllerPlugin::RadarScreen::RadarRenderableCollection& renderables;

        // Command for resetting the visuals
        const std::string resetCommand = ".ukcp resetvisuals";
    };
} // namespace UKControllerPlugin::RadarScreen
