#include "Runway.h"
#include "geometry/Angle.h"
#include "headings/Heading.h"

namespace UKControllerPlugin::Runway {

    Runway::Runway(int id, int airfieldId, std::string identifier, int heading, EuroScopePlugIn::CPosition threshold)
        // TODO: Remove hard-coded threshold elevation and glideslope angle
        : id(id), airfieldId(airfieldId), identifier(std::move(identifier)), heading(heading),
          headingRadians(Geometry::DegreesToRadians(heading)),
          perpendicularHeading(Headings::PerpendicularHeading(heading)),
          perpendicularHeadingRadians(Geometry::DegreesToRadians(perpendicularHeading)),
          runwayHeadingLineSlope(Geometry::Slope(headingRadians)),
          runwayPerpendicularHeadingLineSlope(Geometry::Slope(perpendicularHeadingRadians)), threshold(threshold),
          thresholdElevation(196), glideslopeAngleRadians(Geometry::DegreesToRadians(3))
    {
    }

    auto Runway::Id() const -> int
    {
        return id;
    }

    auto Runway::AirfieldId() const -> int
    {
        return airfieldId;
    }

    auto Runway::Identifier() const -> const std::string&
    {
        return identifier;
    }

    auto Runway::Heading() const -> int
    {
        return heading;
    }

    auto Runway::Threshold() const -> const EuroScopePlugIn::CPosition&
    {
        return threshold;
    }

    auto Runway::GlideslopeAltitudeAtDistance(const double distanceInNauticalMiles) const -> int
    {
        return (glideslopeAngleRadians * (distanceInNauticalMiles * 6076.12)) + thresholdElevation;
    }

    auto Runway::RunwayHeadingLineSlope() const -> double
    {
        return runwayHeadingLineSlope;
    }

    auto Runway::RunwayPerpendicularHeadingLineSlope() const -> double
    {
        return runwayPerpendicularHeadingLineSlope;
    }
} // namespace UKControllerPlugin::Runway
