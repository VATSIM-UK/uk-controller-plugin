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
                EuroscopeSectorFileElementWrapper(EuroScopePlugIn::CSectorElement originalData);

                // Inherited via EuroscopeSectorFileElementInterface
                virtual std::string Airport(void) const override;
                virtual std::string Name(void) const override;
                virtual bool Runway1ActiveForDepartures(void) override;
                virtual bool Runway1ActiveForArrivals(void) override;
                virtual bool Runway2ActiveForDepartures(void) override;
                virtual bool Runway2ActiveForArrivals(void) override;
                virtual std::string Runway1ComponentName(void) override;
                virtual std::string Runway2ComponentName(void) override;
                virtual int Runway1Heading(void) override;
                virtual int Runway2Heading(void) override;
                virtual std::string Runway1Identifier(void) const override;
                virtual std::string Runway2Identifier(void) const override;

            private:

                // The original ES class
                EuroScopePlugIn::CSectorElement originalData;
        };
    }  // naemspace Euroscope
}  // namespace UKControllerPlugin
