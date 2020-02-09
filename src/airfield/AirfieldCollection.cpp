#include "pch/stdafx.h"
#include "airfield/AirfieldCollection.h"

using UKControllerPlugin::Airfield::AirfieldModel;

namespace UKControllerPlugin {
    namespace Airfield {

        /*
            Add an airfield to the collection. Throws an exception
            if already added.
        */
        void AirfieldCollection::AddAirfield(std::unique_ptr<AirfieldModel> airfield)
        {
            if (this->airfieldMap.count(airfield->GetIcao()) != 0) {
                throw std::invalid_argument("Airfield " + airfield->GetIcao() + " has already been added.");
            }

            if (!this->IsHomeAirfield(airfield->GetIcao())) {
                throw std::invalid_argument("Airfield " + airfield->GetIcao() + " is not a home airfield.");
            }

            this->airfieldMap.insert({ airfield->GetIcao(), std::move(airfield) });
        }

        /*
            Returns an airfield based on its ICAO code. Throws an exception if not found.
        */
        const AirfieldModel& AirfieldCollection::FetchAirfieldByIcao(std::string icao) const
        {
            if (!this->IsHomeAirfield(icao) || this->GetSize() == 0) {
                throw std::out_of_range("Airfield not found");
            }

            auto iterator = std::find_if(this->airfieldMap.begin(), this->airfieldMap.end(),
                [icao](std::pair<std::string, const std::unique_ptr<AirfieldModel> &> airfield) -> bool {

                return airfield.second->GetIcao().compare(icao) == 0;
            });

            if (iterator == this->airfieldMap.end()) {
                throw std::out_of_range("Airfield not found");
            }

            return *iterator->second;
        }

        /*
            Returns the number of airfields in the collection.
        */
        size_t AirfieldCollection::GetSize(void) const
        {
            return this->airfieldMap.size();
        }

        /*
            Returns true if the airfield is a "home" airfield, that is, the ICAO
            starts with EG.
        */
        bool AirfieldCollection::IsHomeAirfield(std::string icao) const
        {
            return icao.substr(0, 2).compare("EG") == 0;
        }
    }  // namespace Airfield
}  // namespace UKControllerPlugin
