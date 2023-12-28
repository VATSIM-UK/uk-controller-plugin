#include "Runway.h"
#include "geometry/Angle.h"
#include "geometry/Length.h"
#include "headings/Heading.h"

namespace UKControllerPlugin::Runway {

    Runway::Runway(
        int id,
        int airfieldId,
        std::string identifier,
        int heading,
        EuroScopePlugIn::CPosition threshold,
        int thresholdElevation,
        double glideslopeAngle)
        // TODO: Remove hard-coded airfield identifier
        : id(id), airfieldId(airfieldId), airfieldIdentifier("EGKK"), identifier(std::move(identifier)),
          heading(heading), headingRadians(Geometry::DegreesToRadians(heading)),
          perpendicularHeading(Headings::PerpendicularHeading(heading)),
          perpendicularHeadingRadians(Geometry::DegreesToRadians(perpendicularHeading)),
          runwayHeadingLineSlope(Geometry::Slope(headingRadians)),
          runwayPerpendicularHeadingLineSlope(Geometry::Slope(perpendicularHeadingRadians)), threshold(threshold),
          thresholdElevation(thresholdElevation), glideslopeAngle(glideslopeAngle),
          glideslopeAngleRadians(Geometry::DegreesToRadians(glideslopeAngle))
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

    auto Runway::AirfieldIdentifier() const -> const std::string&
    {
        return airfieldIdentifier;
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
        return (glideslopeAngleRadians * Geometry::NauticalMilesToFeet(distanceInNauticalMiles)) + thresholdElevation;
    }

    auto Runway::RunwayHeadingLineSlope() const -> double
    {
        return runwayHeadingLineSlope;
    }

    auto Runway::RunwayPerpendicularHeadingLineSlope() const -> double
    {
        return runwayPerpendicularHeadingLineSlope;
    }

    auto Runway::ThresholdElevation() const -> int
    {
        return thresholdElevation;
    }

    auto Runway::GlideslopeAngle() const -> double
    {
        return glideslopeAngle;
    }
} // namespace UKControllerPlugin::Runway
