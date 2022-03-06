#include "Navaid.h"

namespace UKControllerPlugin::Navaids {
    auto Navaid::operator==(const Navaid& compare) const -> bool
    {
        return this->identifier == compare.identifier;
    }

    auto Navaid::Navaid::operator!=(const Navaid& compare) const -> bool
    {
        return !(*this == compare);
    }
} // namespace UKControllerPlugin::Navaids
