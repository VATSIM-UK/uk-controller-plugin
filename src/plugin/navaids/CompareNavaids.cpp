#include "navaids/CompareNavaids.h"
#include "navaids/Navaid.h"

using UKControllerPlugin::Navaids::Navaid;

namespace UKControllerPlugin {
    namespace Navaids {

        bool CompareNavaids::operator()(const Navaid& navaid, std::string identifier) const
        {
            return navaid.identifier < identifier;
        }

        bool CompareNavaids::operator()(std::string identifier, const Navaid& navaid) const
        {
            return identifier < navaid.identifier;
        }

        bool CompareNavaids::operator()(const Navaid& a, const Navaid& b) const
        {
            return a.identifier < b.identifier;
        }
    } // namespace Navaids
} // namespace UKControllerPlugin
