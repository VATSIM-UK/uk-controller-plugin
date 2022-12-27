#include "EuroscopeCoordinateWrapper.h"

namespace UKControllerPlugin::Euroscope {
    EuroscopeCoordinateWrapper::EuroscopeCoordinateWrapper(EuroScopePlugIn::CPosition original) : original(original)
    {
    }

    auto EuroscopeCoordinateWrapper::DirectionTo(const EuroscopeCoordinateInterface& coordinate) const -> double
    {
        return original.DirectionTo(coordinate.ToEuroscopePosition());
    }

    auto EuroscopeCoordinateWrapper::ToEuroscopePosition() const -> const EuroScopePlugIn::CPosition
    {
        return original;
    }
} // namespace UKControllerPlugin::Euroscope
