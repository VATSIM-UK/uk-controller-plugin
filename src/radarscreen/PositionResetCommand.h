#pragma once
#include "command/CommandHandlerInterface.h"
#include "RadarScreen/RadarRenderableCollection.h"

namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            Processes commands to reset the position of renderables
        */
        class PositionResetCommand : public UKControllerPlugin::Command::CommandHandlerInterface
        {
            public:
                PositionResetCommand(const UKControllerPlugin::RadarScreen::RadarRenderableCollection & renderables);

                // Inherited via CommandHandlerInterface
                bool ProcessCommand(std::string command) override;

                // Command for resetting the visuals
                const std::string resetCommand = ".ukcp resetvisuals";

            private:
                
                // Radar renderables
                const UKControllerPlugin::RadarScreen::RadarRenderableCollection & renderables;
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
