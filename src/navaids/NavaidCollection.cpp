#include "pch/stdafx.h"
#include "navaids/NavaidCollection.h"

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

    }  // namespace Navaids
}  // namespace UKControllerPlugin
