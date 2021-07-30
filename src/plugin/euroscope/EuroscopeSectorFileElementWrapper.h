#pragma once
#include "euroscope/EuroscopeSectorFileElementInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {


        /*
            An implementation for a class that wraps EuroScopes CSectorElement class
        */
        class EuroscopeSectorFileElementWrapper :
            public UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface
        {
            public:
                explicit EuroscopeSectorFileElementWrapper(EuroScopePlugIn::CSectorElement originalData);

                // Inherited via EuroscopeSectorFileElementInterface
                std::string Airport(void) const override;
                std::string Name(void) const override;
                bool Runway1ActiveForDepartures(void) override;
                bool Runway1ActiveForArrivals(void) override;
                bool Runway2ActiveForDepartures(void) override;
                bool Runway2ActiveForArrivals(void) override;
                int Runway1Heading(void) override;
                int Runway2Heading(void) override;
                std::string Runway1Identifier(void) const override;
                std::string Runway2Identifier(void) const override;

            private:

                // The original ES class
                EuroScopePlugIn::CSectorElement originalData;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
