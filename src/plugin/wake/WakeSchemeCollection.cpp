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
} // namespace UKControllerPlugin::Wake
