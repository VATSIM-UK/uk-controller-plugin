#pragma once
#include "pch/pch.h"
#include "euroscope/EuroscopeSectorFileElementWrapper.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        EuroscopeSectorFileElementWrapper::EuroscopeSectorFileElementWrapper(
            EuroScopePlugIn::CSectorElement originalData
        )
            : originalData(originalData)
        {
        }

        std::string EuroscopeSectorFileElementWrapper::Airport(void) const
        {
            return this->originalData.GetAirportName();
        }

        std::string EuroscopeSectorFileElementWrapper::Name(void) const
        {
            return this->originalData.GetName();
        }

        bool EuroscopeSectorFileElementWrapper::Runway1ActiveForDepartures(void)
        {
            return this->originalData.IsElementActive(true, 0);
        }

        bool EuroscopeSectorFileElementWrapper::Runway1ActiveForArrivals(void)
        {
            return this->originalData.IsElementActive(false, 0);
        }

        bool EuroscopeSectorFileElementWrapper::Runway2ActiveForDepartures(void)
        {
            return this->originalData.IsElementActive(true, 1);
        }

        bool EuroscopeSectorFileElementWrapper::Runway2ActiveForArrivals(void)
        {
            return this->originalData.IsElementActive(false, 1);
        }

        int EuroscopeSectorFileElementWrapper::Runway1Heading(void)
        {
            return this->originalData.GetRunwayHeading(0);
        }

        int EuroscopeSectorFileElementWrapper::Runway2Heading(void)
        {
            return this->originalData.GetRunwayHeading(1);
        }

        std::string EuroscopeSectorFileElementWrapper::Runway1Identifier(void) const
        {
            return this->originalData.GetRunwayName(0);
        }

        std::string EuroscopeSectorFileElementWrapper::Runway2Identifier(void) const
        {
            return this->originalData.GetRunwayName(1);
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
