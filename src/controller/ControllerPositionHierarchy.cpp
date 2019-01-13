#include "pch/stdafx.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin {
    namespace Controller {

        ControllerPositionHierarchy::ControllerPositionHierarchy(void)
        {

        }

        /*
            Adds a position to the hierarchy.
        */
        void ControllerPositionHierarchy::AddPosition(const ControllerPosition & position)
        {
            this->positions.push_back(std::reference_wrapper<const ControllerPosition>(position));
        }

        /*
            Returns the size of the hierarchy.
        */
        size_t ControllerPositionHierarchy::CountPositions(void) const
        {
            return this->positions.size();
        }

        /*
            Returns true iff a given position is found in the hierarchy.
        */
        bool ControllerPositionHierarchy::PositionInHierarchy(const ControllerPosition & position) const
        {
            return std::find_if(
                this->positions.cbegin(),
                this->positions.cend(),
                [position](std::reference_wrapper<const ControllerPosition>(storedPosition)) -> bool {
                    return position == storedPosition.get();
            }) != this->positions.cend();
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
