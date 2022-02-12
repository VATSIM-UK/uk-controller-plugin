#include "AircraftType.h"

namespace UKControllerPlugin::Aircraft {
    AircraftType::AircraftType(int id, std::string icaoCode, std::set<int> wakeCategories)
        : id(id), icaoCode(std::move(icaoCode)), wakeCategories(std::move(wakeCategories))
    {
    }

    auto AircraftType::Id() const -> int
    {
        return id;
    }

    auto AircraftType::IcaoCode() const -> const std::string&
    {
        return icaoCode;
    }

    auto AircraftType::WakeCategories() const -> const std::set<int>&
    {
        return wakeCategories;
    }
} // namespace UKControllerPlugin::Aircraft
