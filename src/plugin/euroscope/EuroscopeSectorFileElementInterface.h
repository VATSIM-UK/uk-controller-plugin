#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        /*
            An interface for a class that wraps EuroScopes CSectorElement class
        */
        class EuroscopeSectorFileElementInterface
        {
            public:

                virtual std::string Airport(void) const = 0;
                virtual std::string Name(void) const = 0;
                virtual bool Runway1ActiveForDepartures(void) = 0;
                virtual bool Runway1ActiveForArrivals(void) = 0;
                virtual bool Runway2ActiveForDepartures(void) = 0;
                virtual bool Runway2ActiveForArrivals(void) = 0;
                virtual int Runway1Heading(void) = 0;
                virtual int Runway2Heading(void) = 0;
                virtual std::string Runway1Identifier(void) const = 0;
                virtual std::string Runway2Identifier(void) const = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
