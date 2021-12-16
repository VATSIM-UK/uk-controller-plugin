#include "ParsedMetar.h"
#include "ParsedMetarCollection.h"

namespace UKControllerPlugin::Metar {

    auto ParsedMetarCollection::Count() const -> size_t
    {
        return this->metars.size();
    }

    auto ParsedMetarCollection::GetForAirfield(const std::string& airfield) const -> std::shared_ptr<ParsedMetar>
    {
        auto metar = this->metars.find(airfield);
        return metar != metars.cend() ? metar->second : nullptr;
    }

    void ParsedMetarCollection::UpdateMetar(const std::shared_ptr<ParsedMetar>& metar)
    {
        this->metars[metar->Airfield()] = metar;
    }
} // namespace UKControllerPlugin::Metar
