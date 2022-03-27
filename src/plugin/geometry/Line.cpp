#include "Line.h"

namespace UKControllerPlugin::Geometry {

    Line::Line(EuroScopePlugIn::CPosition start, EuroScopePlugIn::CPosition end)
        : start(std::move(start)), end(std::move(end))
    {
    }

    auto Line::Start() const -> const EuroScopePlugIn::CPosition&
    {
        return start;
    }

    auto Line::End() const -> const EuroScopePlugIn::CPosition&
    {
        return end;
    }

    auto Line::IntersectsWith(const Line& line) const -> bool
    {
        return this->DLat() != line.DLat() || this->DLong() != line.DLong();
    }

    auto Line::Intersection(const Line& line) const -> EuroScopePlugIn::CPosition
    {
        double denominator = (start.m_Latitude - end.m_Latitude) * (line.start.m_Longitude - line.end.m_Longitude) -
                             (start.m_Longitude - end.m_Longitude) * (line.start.m_Latitude - line.end.m_Latitude);

        if (denominator == 0.0) {
            return {};
        }

        double intersectX = ((start.m_Latitude * end.m_Longitude - start.m_Longitude * end.m_Latitude) *
                                 (line.start.m_Latitude - line.end.m_Latitude) -
                             (start.m_Latitude - end.m_Latitude) * (line.start.m_Latitude * line.end.m_Longitude -
                                                                    line.start.m_Longitude * line.end.m_Latitude)) /
                            denominator;

        double intersectY = ((start.m_Latitude * end.m_Longitude - start.m_Longitude * end.m_Latitude) *
                                 (line.start.m_Longitude - line.end.m_Longitude) -
                             (start.m_Longitude - end.m_Longitude) * (line.start.m_Latitude * line.end.m_Longitude -
                                                                      line.start.m_Longitude * line.end.m_Latitude)) /
                            denominator;

        auto position = EuroScopePlugIn::CPosition();
        position.m_Latitude = intersectX;
        position.m_Longitude = intersectY;
        return position;
    }

    auto Line::DLong() const -> double
    {
        return start.m_Longitude - end.m_Longitude;
    }

    auto Line::DLat() const -> double
    {
        return start.m_Latitude - end.m_Latitude;
    }
} // namespace UKControllerPlugin::Geometry
