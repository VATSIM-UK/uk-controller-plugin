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
                void AddRunway(std::shared_ptr<UKControllerPlugin::Airfield::Runway> runway);
                size_t Count(void) const;
                size_t CountForAirfield(std::string airfield) const;
                void Clear(void);
                const UKControllerPlugin::Airfield::Runway & FetchByIdentifierAndAirfield(
                    std::string identifier,
                    std::string airfield
                ) const;
                const std::shared_ptr<UKControllerPlugin::Airfield::Runway> FetchBySectorFileIdentifier(
                    std::string identifier
                );


                const UKControllerPlugin::Airfield::Runway invalidRunway = { "", "", "", 371 };
            private:

                // A map of sector file identifier to runways
                std::map<std::string, std::shared_ptr <UKControllerPlugin::Airfield::Runway>> runways;

                // A map of airfield to runways
                std::map<std::string, std::set<std::shared_ptr<UKControllerPlugin::Airfield::Runway>>> airfieldMap;
        };
    }  // namespace Airfield
}  // namespace UKControllerPlugin
