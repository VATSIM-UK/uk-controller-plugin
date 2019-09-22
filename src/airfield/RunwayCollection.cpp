#include "pch/stdafx.h"
#include "airfield/RunwayCollection.h"

using UKControllerPlugin::Airfield::Runway;

namespace UKControllerPlugin {
    namespace Airfield {
        
        /*
            Adds a runway to the collection
        */
        void RunwayCollection::AddRunway(std::shared_ptr<UKControllerPlugin::Airfield::Runway> runway)
        {
            if (this->runways.count(runway->sectorfileIdentifier)) {
                LogWarning("Attempted to add duplicate runway: " + runway->sectorfileIdentifier);
                return;
            }

            this->runways[runway->sectorfileIdentifier] = runway;
            this->airfieldMap[runway->airfield].insert(runway);
        }

        size_t RunwayCollection::Count(void) const
        {
            return this->runways.size();
        }

        size_t RunwayCollection::CountForAirfield(std::string airfield) const
        { 
            return this->airfieldMap.count(airfield) ? this->airfieldMap.at(airfield).size() : 0;
        }

        void RunwayCollection::Clear(void)
        {
            this->runways.clear();
            this->airfieldMap.clear();
        }

        /*
            Find a runway by identifier and airfield
        */
        const Runway & RunwayCollection::FetchByIdentifierAndAirfield(std::string identifier, std::string airfield) const
        {
            if (!this->airfieldMap.count(airfield)) {
                return this->invalidRunway;
            }

            std::set<std::shared_ptr<Runway>> airfieldRunways = this->airfieldMap.at(airfield);

            auto runway = std::find_if(
                airfieldRunways.cbegin(),
                airfieldRunways.cend(),
                [identifier](const std::shared_ptr<Runway> & runway) -> bool {
                    return identifier == runway->identifier;
                }
            );

            return runway == airfieldRunways.cend() ? this->invalidRunway : **runway;
        }

        /*
            Fetch the runway in a form that may be updated - e.g.
        */
        const std::shared_ptr<Runway> RunwayCollection::FetchBySectorFileIdentifier(std::string identifier)
        {
            return this->runways.count(identifier) ? this->runways.at(identifier) : nullptr;
        }
    }  // namespace Airfield
}  // namespace UKControllerPlugin
