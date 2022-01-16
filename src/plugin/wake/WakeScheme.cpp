#include "WakeScheme.h"

namespace UKControllerPlugin::Wake {
    WakeScheme::WakeScheme(
        int id, std::string key, std::string name, std::list<std::shared_ptr<WakeCategory>> categories)
        : id(id), key(std::move(key)), name(std::move(name)), categories(std::move(categories))
    {
    }

    auto WakeScheme::Id() const -> int
    {
        return id;
    }

    auto WakeScheme::Key() const -> const std::string&
    {
        return key;
    }

    auto WakeScheme::Name() const -> const std::string&
    {
        return name;
    }

    auto WakeScheme::Categories() const -> const std::list<std::shared_ptr<WakeCategory>>&
    {
        return categories;
    }
} // namespace UKControllerPlugin::Wake
