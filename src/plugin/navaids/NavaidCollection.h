#pragma once
#include "navaids/Navaid.h"
#include "navaids/CompareNavaids.h"

namespace UKControllerPlugin {
    namespace Navaids {

        /*
            A collection of all the navaids.
        */
        class NavaidCollection
        {
            public:
            void AddNavaid(Navaid navaid);
            size_t Count(void) const;
            const UKControllerPlugin::Navaids::Navaid& GetByIdentifier(std::string identifier) const;

            const Navaid invalidNavaid = {0, "INVALID", EuroScopePlugIn::CPosition()};

            typedef std::set<Navaid, CompareNavaids> NavaidList;
            typedef NavaidList::const_iterator const_iterator;
            const_iterator cbegin() const
            {
                return navaids.cbegin();
            }
            const_iterator cend() const
            {
                return navaids.cend();
            }

            private:
            // All the navaids
            NavaidList navaids;
        };
    } // namespace Navaids
} // namespace UKControllerPlugin
