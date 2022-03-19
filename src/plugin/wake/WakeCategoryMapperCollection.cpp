#include "WakeCategoryMapperCollection.h"

namespace UKControllerPlugin::Wake {

    void WakeCategoryMapperCollection::Add(int wakeSchemeId, std::shared_ptr<WakeCategoryMapperInterface> mapper)
    {
        if (mappers.count(wakeSchemeId) != 0) {
            LogError("Duplicate wake category mapper added");
            return;
        }

        mappers[wakeSchemeId] = mapper;
    }

    auto WakeCategoryMapperCollection::Get(int wakeSchemeId) const -> std::shared_ptr<WakeCategoryMapperInterface>
    {
        return mappers.count(wakeSchemeId) != 0 ? mappers.at(wakeSchemeId) : nullptr;
    }

    auto WakeCategoryMapperCollection::Count() const -> size_t
    {
        return mappers.size();
    }
} // namespace UKControllerPlugin::Wake
