#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPosition;

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
            const UKControllerPlugin::Controller::ControllerPosition& normalisedPosition,
            bool isUser);
        ActiveCallsign(const ActiveCallsign& copyFrom);
        [[nodiscard]] auto GetCallsign() const -> const std::string&;
        [[nodiscard]] auto GetControllerName() const -> const std::string&;
        [[nodiscard]] auto GetNormalisedPosition() const -> const UKControllerPlugin::Controller::ControllerPosition&;
        [[nodiscard]] auto GetIsUser() const -> bool;
        auto operator<(const ActiveCallsign& comparator) const -> bool;
        auto operator>(const ActiveCallsign& comparator) const -> bool;
        auto operator==(const ActiveCallsign& comparator) const -> bool;
        auto operator!=(const ActiveCallsign& comparator) const -> bool;

        private:
        // The callsign as it is logged in.
        std::string callsign;

        // The name of the controller.
        std::string controllerName;

        // The normalised callsign of the controller - matching it to a recognised controller position.
        const UKControllerPlugin::Controller::ControllerPosition& normalisedPosition;

        // Is this callsign the users?
        bool isUser;
    };
} // namespace UKControllerPlugin::Controller
