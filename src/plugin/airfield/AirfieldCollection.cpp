#include "AirfieldCollection.h"
#include "AirfieldModel.h"

namespace UKControllerPlugin::Airfield {

    AirfieldCollection::AirfieldCollection() = default;
    AirfieldCollection::~AirfieldCollection() = default;
    AirfieldCollection::AirfieldCollection(AirfieldCollection&&) noexcept = default;
    auto AirfieldCollection::operator=(AirfieldCollection&&) noexcept -> AirfieldCollection& = default;

    /*
        Add an airfield to the collection. Throws an exception
        if already added.
    */
    void AirfieldCollection::AddAirfield(std::unique_ptr<AirfieldModel> airfield)
    {
        if (this->airfieldMap.count(airfield->GetIcao()) != 0) {
            throw std::invalid_argument("Airfield " + airfield->GetIcao() + " has already been added.");
        }

        if (!IsHomeAirfield(airfield->GetIcao())) {
            throw std::invalid_argument("Airfield " + airfield->GetIcao() + " is not a home airfield.");
        }

        this->airfieldMap.insert({airfield->GetIcao(), std::move(airfield)});
    }

    /*
        Returns an airfield based on its ICAO code. Throws an exception if not found.
    */
    auto AirfieldCollection::FetchAirfieldByIcao(const std::string& icao) const -> const AirfieldModel&
    {
        if (!IsHomeAirfield(icao) || this->GetSize() == 0) {
            throw std::out_of_range("Airfield not found");
        }

        auto iterator = std::find_if(
            this->airfieldMap.begin(),
            this->airfieldMap.end(),
            [icao](const std::pair<std::string, const std::unique_ptr<AirfieldModel>&>& airfield) -> bool {
                return airfield.second->GetIcao() == icao;
            });

        if (iterator == this->airfieldMap.end()) {
            throw std::out_of_range("Airfield not found");
        }

        return *iterator->second;
    }

    /*
        Returns the number of airfields in the collection.
    */
    auto AirfieldCollection::GetSize() const -> size_t
    {
        return this->airfieldMap.size();
    }

    /*
        Returns true if the airfield is a "home" airfield, that is, the ICAO
        starts with EG.
    */
    auto AirfieldCollection::IsHomeAirfield(const std::string& icao) -> bool
    {
        return icao.substr(0, 2) == "EG";
    }
} // namespace UKControllerPlugin::Airfield
