#include "pch/pch.h"

#include "euroscope/EuroscopeSectorFileElementWrapper.h"

namespace UKControllerPlugin::Euroscope {

    EuroscopeSectorFileElementWrapper::EuroscopeSectorFileElementWrapper(EuroScopePlugIn::CSectorElement originalData)
        : originalData(originalData)
    {
    }

    auto EuroscopeSectorFileElementWrapper::Airport() const -> std::string
    {
        return this->originalData.GetAirportName();
    }

    auto EuroscopeSectorFileElementWrapper::Name() const -> std::string
    {
        return this->originalData.GetName();
    }

    auto EuroscopeSectorFileElementWrapper::Runway1ActiveForDepartures() -> bool
    {
        return this->originalData.IsElementActive(true, 0);
    }

    auto EuroscopeSectorFileElementWrapper::Runway1ActiveForArrivals() -> bool
    {
        return this->originalData.IsElementActive(false, 0);
    }

    auto EuroscopeSectorFileElementWrapper::Runway2ActiveForDepartures() -> bool
    {
        return this->originalData.IsElementActive(true, 1);
    }

    auto EuroscopeSectorFileElementWrapper::Runway2ActiveForArrivals() -> bool
    {
        return this->originalData.IsElementActive(false, 1);
    }

    auto EuroscopeSectorFileElementWrapper::Runway1Heading() -> int
    {
        return this->originalData.GetRunwayHeading(0);
    }

    auto EuroscopeSectorFileElementWrapper::Runway2Heading() -> int
    {
        return this->originalData.GetRunwayHeading(1);
    }

    auto EuroscopeSectorFileElementWrapper::Runway1Identifier() const -> std::string
    {
        return this->originalData.GetRunwayName(0);
    }

    auto EuroscopeSectorFileElementWrapper::Runway2Identifier() const -> std::string
    {
        return this->originalData.GetRunwayName(1);
    }
} // namespace UKControllerPlugin::Euroscope
