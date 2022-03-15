#include "WakeCategoryMapperCollection.h"

namespace UKControllerPlugin::Wake {

    void WakeCategoryMapperCollection::Add(int wakeSchemeId, std::shared_ptr<WakeCategoryMapperInterface> mapper)
    {
        assert(mappers.count(wakeSchemeId) == 0 && "Duplicate wake category mapper added");
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
