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
        [[nodiscard]] auto GlideslopeAltitudeAtDistance(const double distanceInNauticalMiles) const -> int;
        [[nodiscard]] auto RunwayHeadingLineSlope() const -> double;
        [[nodiscard]] auto RunwayPerpendicularHeadingLineSlope() const -> double;

        private:
        // A convenient calculation of pi
        const double pi = std::atan(1) * 4;

        // The id of the runway in the API
        int id;

        // The id of the airfield that this runway is at
        int airfieldId;

        // The identifier of the runway - e.g 27L
        std::string identifier;

        // The heading of the runway
        int heading;

        // The heading of the runway in radians
        double headingRadians;

        // The perpendicular heading of the runway
        int perpendicularHeading;

        // The perpendicular heading of the runway in radians
        double perpendicularHeadingRadians;

        // The slope of the runway heading line
        double runwayHeadingLineSlope;

        // The slope of the runway perpendicular heading line
        double runwayPerpendicularHeadingLineSlope;

        // The coordinates of the runway threshold
        EuroScopePlugIn::CPosition threshold;

        // The elevation of the runway threshold
        int thresholdElevation;

        // The glideslope angle (in radians)
        double glideslopeAngleRadians;
    };
} // namespace UKControllerPlugin::Runway
