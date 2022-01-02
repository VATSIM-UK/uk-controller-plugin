#include "ParsedMetar.h"
#include "ParsedMetarCollection.h"

namespace UKControllerPlugin::Metar {

    auto ParsedMetarCollection::Count() const -> size_t
    {
        auto lock = std::lock_guard<std::mutex>(containerLock);
        return this->metars.size();
    }

    auto ParsedMetarCollection::GetForAirfield(const std::string& airfield) const -> std::shared_ptr<ParsedMetar>
    {
        auto lock = std::lock_guard<std::mutex>(containerLock);
        auto metar = this->metars.find(airfield);
        return metar != metars.cend() ? metar->second : nullptr;
    }

    void ParsedMetarCollection::UpdateMetar(const std::shared_ptr<ParsedMetar>& metar)
    {
        auto lock = std::lock_guard<std::mutex>(containerLock);
        this->metars[metar->Airfield()] = metar;
    }
} // namespace UKControllerPlugin::Metar
