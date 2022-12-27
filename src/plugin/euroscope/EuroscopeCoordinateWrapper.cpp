#include "EuroscopeCoordinateWrapper.h"

namespace UKControllerPlugin::Euroscope {
    EuroscopeCoordinateWrapper::EuroscopeCoordinateWrapper(EuroScopePlugIn::CPosition original)
        : original(std::move(original))
    {
    }

    auto EuroscopeCoordinateWrapper::DirectionTo(const EuroscopeCoordinateInterface& coordinate) const
        -> double override
    {
        return original.DirectionTo(coordinate.ToEuroScopePosition());
    }

    auto EuroscopeCoordinateWrapper::ToEuroscopePosition() const -> const EuroScopePlugIn::CPosition&
    {
        return original;
    }

} // namespace UKControllerPlugin::Euroscope