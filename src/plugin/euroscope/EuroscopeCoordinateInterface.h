#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroscopeCoordinateInterface
    {
        public:
        virtual ~EuroscopeCoordinateInterface() = default;
        [[nodiscard]] virtual auto DirectionTo(const EuroscopeCoordinateInterface& coordinate) const -> double = 0;
        [[nodiscard]] virtual auto ToEuroscopePosition() const -> const EuroScopePlugIn::CPosition = 0;
    };
} // namespace UKControllerPlugin::Euroscope
