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

                const Navaid invalidNavaid = { 0, "INVALID" };

            private:
                // All the navaids
                std::set<Navaid, CompareNavaids> navaids;
        };
    }  // namespace Navaids
}  // namespace UKControllerPlugin
