#include "MappingElement.h"
#include "MappingElementManager.h"

namespace UKControllerPlugin::Mapping {

    void MappingElementManager::Add(std::shared_ptr<MappingElement> element)
    {
        allElements.insert(element);
        activeElements.insert(element);
    }

    auto MappingElementManager::Count() const -> size_t
    {
        return allElements.size();
    }

    auto MappingElementManager::GetByTypeAndId(const std::string& type, int id) const -> std::shared_ptr<MappingElement>
    {
        auto element = std::find_if(
            allElements.cbegin(), allElements.cend(), [&type, &id](const std::shared_ptr<MappingElement>& element) {
                return element->Type() == type && element->Id() == id;
            });

        return element == allElements.cend() ? nullptr : *element;
    }
} // namespace UKControllerPlugin::Mapping
