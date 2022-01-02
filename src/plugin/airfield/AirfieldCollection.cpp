#include "AirfieldCollection.h"
#include "AirfieldModel.h"

namespace UKControllerPlugin::Airfield {

    AirfieldCollection::AirfieldCollection() = default;
    AirfieldCollection::~AirfieldCollection() = default;
    AirfieldCollection::AirfieldCollection(AirfieldCollection&&) noexcept = default;
    auto AirfieldCollection::operator=(AirfieldCollection&&) noexcept -> AirfieldCollection& = default;

    void AirfieldCollection::AddAirfield(std::shared_ptr<AirfieldModel> airfield)
    {
        if (this->airfieldMap.count(airfield->Id()) != 0) {
            LogWarning("Airfield " + airfield->Icao() + " has already been added");
            return;
        }

        this->airfieldMap.insert({airfield->Id(), std::move(airfield)});
    }

    /*
        Returns an airfield based on its ICAO code. Throws an exception if not found.
    */
    auto AirfieldCollection::FetchAirfieldByIcao(const std::string& icao) const -> std::shared_ptr<AirfieldModel>
    {
        auto airfield = std::find_if(
            this->airfieldMap.cbegin(),
            this->airfieldMap.cend(),
            [&icao](const std::pair<int, std::shared_ptr<AirfieldModel>>& airfield) -> bool {
                return airfield.second->Icao() == icao;
            });

        return airfield == this->airfieldMap.cend() ? nullptr : airfield->second;
    }

    /*
        Returns the number of airfields in the collection.
    */
    auto AirfieldCollection::GetSize() const -> size_t
    {
        return this->airfieldMap.size();
    }

    void AirfieldCollection::ForEach(const std::function<void(const AirfieldModel&)>& callback) const
    {
        for (const auto& airfieldMapping : this->airfieldMap) {
            callback(*airfieldMapping.second);
        }
    }

    auto AirfieldCollection::FetchById(int id) const -> std::shared_ptr<AirfieldModel>
    {
        return this->airfieldMap.count(id) != 0 ? this->airfieldMap.at(id) : nullptr;
    }
} // namespace UKControllerPlugin::Airfield
