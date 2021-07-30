#include "pch/pch.h"
#include "prenote/AbstractPrenote.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPlugin {
    namespace Prenote {


        AbstractPrenote::AbstractPrenote(std::unique_ptr<ControllerPositionHierarchy> controllers)
            : controllers(std::move(controllers))
        {
        }

        AbstractPrenote::~AbstractPrenote()
        {
        }

        /*
            Returns the order in which controllers should be tried for pre-note.
        */
        const ControllerPositionHierarchy & AbstractPrenote::GetControllers(void) const
        {
            return *this->controllers;
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
