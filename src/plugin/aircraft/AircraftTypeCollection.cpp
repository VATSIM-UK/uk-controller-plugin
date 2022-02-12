#include "AircraftType.h"
#include "AircraftTypeCollection.h"

namespace UKControllerPlugin::Aircraft {

    void AircraftTypeCollection::Add(std::shared_ptr<AircraftType> type)
    {
        if (types.count(type->Id()) != 0) {
            LogWarning("Duplicate aircraft type added");
            return;
        }

        types[type->Id()] = type;
    }

    auto AircraftTypeCollection::Count() const -> size_t
    {
        return types.size();
    }

    auto AircraftTypeCollection::GetByIcaoCode(const std::string& code) const -> std::shared_ptr<AircraftType>
    {
        auto type = std::find_if(
            types.cbegin(),
            types.cend(),
            [&code](const std::pair<int, std::shared_ptr<AircraftType>>& typePair) -> bool {
                return typePair.second->IcaoCode() == code;
            });
        return type == types.cend() ? nullptr : type->second;
    }
} // namespace UKControllerPlugin::Aircraft
