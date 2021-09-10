#include "Navaid.h"

namespace UKControllerPlugin::Navaids {
    auto Navaid::operator==(const Navaid& compare) const -> bool
    {
        return this->identifier == compare.identifier;
    }
} // namespace UKControllerPlugin::Navaids
