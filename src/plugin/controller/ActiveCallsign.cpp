#include "pch/stdafx.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin {
    namespace Controller {

        ActiveCallsign::ActiveCallsign(
            std::string callsign,
            std::string controllerName,
            const ControllerPosition & normalisedPosition
        ) : normalisedPosition(normalisedPosition)
        {
            this->callsign = callsign;
            this->controllerName = controllerName;
        }

        /*
            Copy constructor
        */
        ActiveCallsign::ActiveCallsign(const ActiveCallsign & copyFrom)
            : normalisedPosition(copyFrom.normalisedPosition)
        {
            this->callsign = copyFrom.callsign;
            this->controllerName = copyFrom.controllerName;
        }

        /*
            Destructor, nothing to do here at the moment.
        */
        ActiveCallsign::~ActiveCallsign(void)
        {
        }

        const std::string ActiveCallsign::GetCallsign(void) const
        {
            return this->callsign;
        }

        const std::string ActiveCallsign::GetControllerName(void) const
        {
            return this->controllerName;
        }

        const ControllerPosition & ActiveCallsign::GetNormalisedPosition(void) const
        {
            return this->normalisedPosition;
        }

        /*
            Compares the objects by their callsign.
        */
        bool ActiveCallsign::operator<(const ActiveCallsign & comparator) const
        {
            return this->callsign < comparator.callsign;
        }

        /*
            Compares the objects by their callsign.
        */
        bool ActiveCallsign::operator>(const ActiveCallsign & comparator) const
        {
            return this->callsign > comparator.callsign;
        }

        /*
            Returns true if the callsigns match.
        */
        bool ActiveCallsign::operator==(const ActiveCallsign & comparator) const
        {
            return this->callsign == comparator.callsign &&
                this->normalisedPosition == comparator.normalisedPosition;
        }

        /*
            Literally just the inverse of equality.
        */
        bool ActiveCallsign::operator!=(const ActiveCallsign & comparator) const
        {
            return !this->operator==(comparator);
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
