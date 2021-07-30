#pragma once
#include "pch/pch.h"
#include "radarscreen/PositionResetCommand.h"

namespace UKControllerPlugin {
    namespace RadarScreen {

        PositionResetCommand::PositionResetCommand(const RadarRenderableCollection & renderables)
            : renderables(renderables)
        {

        }

        /*
            Process commands
        */
        bool PositionResetCommand::ProcessCommand(std::string command)
        {
            if (command != this->resetCommand) {
                return false;
            }

            this->renderables.ResetPosition();
            return true;
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
