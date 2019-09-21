#include "pch/stdafx.h"
#include "airfield/RunwayCollection.h"

using UKControllerPlugin::Airfield::Runway;

namespace UKControllerPlugin {
    namespace Airfield {
        
        /*
            Adds a runway to the collection
        */
        void RunwayCollection::AddRunway(std::unique_ptr<UKControllerPlugin::Airfield::Runway> runway)
        {
            this->runways[runway->airfield].insert(std::move(runway));
        }

        /*
            Get all the runways for an airfield
        */
        std::set<UKControllerPlugin::Airfield::Runway> RunwayCollection::GetAllForAirfield(std::string airfield) const
        {
            if (!this->runways.count(airfield)) {
                return {};
            }

            std::set<Runway> returnRunways;
            std::set<std::unique_ptr<Runway>> storedRunways = this->runways.at(airfield);

            for (
                std::set<std::unique_ptr<Runway>>::const_iterator it = storedRunways.cbegin();
                it != storedRunways.cend();
                ++it
            ) {
                returnRunways.insert(**it);
            }

            return returnRunways;
        }

        /*
            Find a runway by identifier and airfield
        */
        const Runway & RunwayCollection::FetchByIdentifierAndAirfield(std::string identifier, std::string airfield) const
        {
            if (!this->runways.count(airfield)) {
                return this->invalidRunway;
            }

            std::set<std::unique_ptr<Runway>> airfieldRunways = this->runways.at(airfield);

            auto runway = std::find_if(
                airfieldRunways.cbegin(),
                airfieldRunways.cend(),
                [identifier](const std::unique_ptr<Runway> & runway) -> bool {
                    return identifier == runway->identifier;
                }
            );

            return runway == airfieldRunways.cend() ? this->invalidRunway : **runway;
        }

        /*
            Replace a runway
        */
        void RunwayCollection::ReplaceRunway(std::unique_ptr<UKControllerPlugin::Airfield::Runway> runway)
        {
            if (this->runways.count(runway->airfield)) {
                std::set<std::unique_ptr<Runway>> airfieldRunways = this->runways.at(runway->airfield);
                std::string identifier = runway->identifier;
                auto existingRunway = std::find_if(
                    airfieldRunways.cbegin(),
                    airfieldRunways.cend(),
                    [identifier](const std::unique_ptr<Runway> & runway) -> bool {
                        return identifier == runway->identifier;
                    }
                );

                if (existingRunway != airfieldRunways.cend()) {
                    this->runways.at(runway->airfield).erase(existingRunway);
                }
            }

            this->runways[runway->airfield].insert(std::move(runway));
        }
    }  // namespace Airfield
}  // namespace UKControllerPlugin
