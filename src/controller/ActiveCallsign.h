#pragma once

namespace UKControllerPlugin {
    namespace Controller {
        class ControllerPosition;
    }  // namespace Controller
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Controller {

        /*
            A class that describes an "Active" controller position, which
            will persist longer than the Euroscope equivalent and allow
            us to track who's online without having to poll back to ES.
        */
        class ActiveCallsign
        {
            public:
                ActiveCallsign(
                    std::string callsign,
                    std::string controllerName,
                    const UKControllerPlugin::Controller::ControllerPosition & normalisedPosition
                );
                ActiveCallsign(const ActiveCallsign & copyFrom);
                ~ActiveCallsign(void);
                const std::string GetCallsign(void) const;
                const std::string GetControllerName(void) const;
                const UKControllerPlugin::Controller::ControllerPosition & GetNormalisedPosition(void) const;
                bool operator<(const ActiveCallsign & comparator) const;
                bool operator>(const ActiveCallsign & comparator) const;
                bool operator==(const ActiveCallsign & comparator) const;
                bool operator!=(const ActiveCallsign & comparator) const;

            private:

                // The callsign as it is logged in.
                std::string callsign;

                // The name of the controller.
                std::string controllerName;

                // The normalised callsign of the controller - matching it to a recognised controller position.
                const UKControllerPlugin::Controller::ControllerPosition & normalisedPosition;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
