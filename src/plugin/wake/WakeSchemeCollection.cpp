#include "WakeScheme.h"
#include "WakeSchemeCollection.h"

namespace UKControllerPlugin::Wake {

    void WakeSchemeCollection::Add(std::shared_ptr<WakeScheme> scheme)
    {
        if (schemes.count(scheme->Id()) != 0) {
            LogWarning("Duplicate wake scheme added");
            return;
        }

        schemes[scheme->Id()] = scheme;
    }

    auto WakeSchemeCollection::Count() const -> size_t
    {
        return schemes.size();
    }

    auto WakeSchemeCollection::GetByKey(const std::string& key) const -> std::shared_ptr<WakeScheme>
    {
        auto scheme = std::find_if(
            schemes.cbegin(),
            schemes.cend(),
            [&key](const std::pair<int, std::shared_ptr<WakeScheme>>& schemePair) -> bool {
                return schemePair.second->Key() == key;
            });

        return scheme == schemes.cend() ? nullptr : scheme->second;
    }

    auto WakeSchemeCollection::GetByName(const std::string& name) const -> std::shared_ptr<WakeScheme>
    {
        auto scheme = std::find_if(
            schemes.cbegin(),
            schemes.cend(),
            [&name](const std::pair<int, std::shared_ptr<WakeScheme>>& schemePair) -> bool {
                return schemePair.second->Name() == name;
            });

        return scheme == schemes.cend() ? nullptr : scheme->second;
    }

    auto WakeSchemeCollection::FirstWhere(const std::function<bool(const WakeScheme&)> predicate) const
        -> std::shared_ptr<WakeScheme>
    {
        for (const auto& scheme : schemes) {
            if (predicate(*scheme.second)) {
                return scheme.second;
            }
        }

        return nullptr;
    }

    void WakeSchemeCollection::ForEach(const std::function<void(const WakeScheme&)> callback) const
    {
        for (const auto& scheme : schemes) {
            callback(*scheme.second);
        }
    }

    auto WakeSchemeCollection::GetById(int id) const -> std::shared_ptr<WakeScheme>
    {
        auto scheme = std::find_if(
            schemes.cbegin(),
            schemes.cend(),
            [&id](const std::pair<int, std::shared_ptr<WakeScheme>>& schemePair) -> bool {
                return schemePair.second->Id() == id;
            });

        return scheme == schemes.cend() ? nullptr : scheme->second;
    }
} // namespace UKControllerPlugin::Wake
