#pragma once

namespace UKControllerPlugin::Runway {
    /**
     * Represents a runway.
     */
    class Runway
    {
        public:
        Runway(int id, int airfieldId, std::string identifier, int heading, EuroScopePlugIn::CPosition threshold);
        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto AirfieldId() const -> int;
        [[nodiscard]] auto Identifier() const -> const std::string&;
        [[nodiscard]] auto Heading() const -> int;
        [[nodiscard]] auto Threshold() const -> const EuroScopePlugIn::CPosition&;

        private:
        // The id of the runway in the API
        int id;

        // The id of the airfield that this runway is at
        int airfieldId;

        // The identifier of the runway - e.g 27L
        std::string identifier;

        // The heading of the runway
        int heading;

        // The coordinates of the runway threshold
        EuroScopePlugIn::CPosition threshold;
    };
} // namespace UKControllerPlugin::Runway
