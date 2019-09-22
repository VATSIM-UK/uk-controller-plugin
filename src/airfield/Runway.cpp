#pragma once
#include "pch/stdafx.h"
#include "airfield/Runway.h"

namespace UKControllerPlugin {
    namespace Airfield {

        Runway::Runway(
            std::string sectorfileIdentifier,
            std::string airfield,
            std::string identifier,
            unsigned int heading,
            bool activeForDepartures,
            bool activeForArrivals
        )
            : sectorfileIdentifier(sectorfileIdentifier), airfield(airfield), identifier(identifier), heading(heading),
            activeForDepartures(activeForDepartures), activeForArrivals(activeForArrivals)
        {

        }

        /*
            Returns whether or not the runway is active at all.
        */
        bool Runway::Active(void) const
        {
            return this->activeForArrivals || this->activeForDepartures;
        }

        bool Runway::ActiveForDepartures(void) const
        {
            return this->activeForDepartures;
        }

        bool Runway::ActiveForArrivals(void) const
        {
            return this->activeForArrivals;
        }

        void Runway::SetActiveForDepartures(bool active)
        {
            this->activeForDepartures = active;
        }

        void Runway::SetActiveForArrivals(bool active)
        {
            this->activeForArrivals = active;
        }

        bool Runway::operator<(const UKControllerPlugin::Airfield::Runway & compare) const
        {
            return this->sectorfileIdentifier < compare.sectorfileIdentifier;
        }

        bool Runway::operator==(const UKControllerPlugin::Airfield::Runway & compare) const
        {
            return this->sectorfileIdentifier == compare.sectorfileIdentifier;
        }
    }  // namespace Airfield
}  // namespace UKControllerPlugin
