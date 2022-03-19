#pragma once

namespace UKControllerPlugin::Geometry {
    class Line
    {
        public:
        Line(EuroScopePlugIn::CPosition start, EuroScopePlugIn::CPosition end);
        [[nodiscard]] auto Start() const -> const EuroScopePlugIn::CPosition&;
        [[nodiscard]] auto End() const -> const EuroScopePlugIn::CPosition&;
        [[nodiscard]] auto IntersectsWith(const Line& line) const -> bool;
        [[nodiscard]] auto Intersection(const Line& line) const -> EuroScopePlugIn::CPosition;

        private:
        [[nodiscard]] auto DLat() const -> double;
        [[nodiscard]] auto DLong() const -> double;

        EuroScopePlugIn::CPosition start;

        EuroScopePlugIn::CPosition end;
    };
} // namespace UKControllerPlugin::Geometry
