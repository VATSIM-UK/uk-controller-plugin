#include "NavaidCollection.h"

namespace UKControllerPlugin {
    namespace Navaids {

        void NavaidCollection::AddNavaid(Navaid navaid)
        {
            if (!this->navaids.insert(navaid).second) {
                LogWarning("Duplicate navaid detected, skipping: " + navaid.identifier);
            }
        }

        size_t NavaidCollection::Count(void) const
        {
            return this->navaids.size();
        }

        const Navaid& NavaidCollection::GetByIdentifier(std::string identifier) const
        {
            auto navaid = this->navaids.find(identifier);
            return navaid == this->navaids.cend() ? this->invalidNavaid : *navaid;
        }

        auto NavaidCollection::Get(int id) const -> const UKControllerPlugin::Navaids::Navaid&
        {
            auto navaid = std::find_if(
                this->navaids.cbegin(), this->navaids.cend(), [&id](const Navaid& navaid) { return navaid.id == id; });
            return navaid == this->navaids.cend() ? this->invalidNavaid : *navaid;
        }
    } // namespace Navaids
} // namespace UKControllerPlugin
