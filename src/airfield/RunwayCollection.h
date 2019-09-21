#pragma once
#include "airfield/Runway.h"

namespace UKControllerPlugin {
    namespace Airfield {

        /*
            Collects together all the runways defined in the users sectorfile.
        */
        class RunwayCollection
        {
            public:
                void AddRunway(std::unique_ptr<UKControllerPlugin::Airfield::Runway> runway);
                std::set<UKControllerPlugin::Airfield::Runway> GetAllForAirfield(std::string airfield) const;
                const UKControllerPlugin::Airfield::Runway & FetchByIdentifierAndAirfield(
                    std::string identifier,
                    std::string airfield
                ) const;
                void ReplaceRunway(std::unique_ptr<UKControllerPlugin::Airfield::Runway> runway);
                size_t GetSize(void) const;


                const UKControllerPlugin::Airfield::Runway invalidRunway = { "", "", "", 371 };
            private:

                // A map of ICAO code to runways
                std::map<std::string, std::set<std::unique_ptr<UKControllerPlugin::Airfield::Runway>>> runways;
        };
    }  // namespace Airfield
}  // namespace UKControllerPlugin
