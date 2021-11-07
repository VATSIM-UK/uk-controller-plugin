#include "ActiveCallsign.h"
#include "ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin::Controller {

    ActiveCallsign::ActiveCallsign(
        std::string callsign, std::string controllerName, const ControllerPosition& normalisedPosition, bool isUser)
        : callsign(std::move(callsign)), controllerName(std::move(controllerName)),
          normalisedPosition(normalisedPosition), isUser(isUser)
    {
    }

    ActiveCallsign::ActiveCallsign(const ActiveCallsign& copyFrom)
        : callsign(copyFrom.callsign), controllerName(copyFrom.controllerName),
          normalisedPosition(copyFrom.normalisedPosition), isUser(copyFrom.isUser)
    {
    }

    auto ActiveCallsign::GetCallsign() const -> const std::string&
    {
        return this->callsign;
    }

    auto ActiveCallsign::GetControllerName() const -> const std::string&
    {
        return this->controllerName;
    }

    auto ActiveCallsign::GetNormalisedPosition() const -> const ControllerPosition&
    {
        return this->normalisedPosition;
    }

    /*
        Compares the objects by their callsign.
    */
    auto ActiveCallsign::operator<(const ActiveCallsign& comparator) const -> bool
    {
        return this->callsign < comparator.callsign;
    }

    /*
        Compares the objects by their callsign.
    */
    auto ActiveCallsign::operator>(const ActiveCallsign& comparator) const -> bool
    {
        return this->callsign > comparator.callsign;
    }

    /*
        Returns true if the callsigns match.
    */
    auto ActiveCallsign::operator==(const ActiveCallsign& comparator) const -> bool
    {
        return this->callsign == comparator.callsign && this->normalisedPosition == comparator.normalisedPosition &&
               this->isUser == comparator.isUser;
    }

    /*
        Literally just the inverse of equality.
    */
    auto ActiveCallsign::operator!=(const ActiveCallsign& comparator) const -> bool
    {
        return !this->operator==(comparator);
    }

    auto ActiveCallsign::GetIsUser() const -> bool
    {
        return this->isUser;
    }
} // namespace UKControllerPlugin::Controller
