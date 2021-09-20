#include "ApiSquawkAllocation.h"

namespace UKControllerPlugin::Squawk {

    [[nodiscard]] auto ApiSquawkAllocation::operator==(const ApiSquawkAllocation& compare) const -> bool
    {
        return this->callsign == compare.callsign && this->squawk == compare.squawk;
    }

    [[nodiscard]] auto ApiSquawkAllocation::operator<(const ApiSquawkAllocation& compare) const -> bool
    {
        return this->callsign < compare.callsign;
    };
} // namespace UKControllerPlugin::Squawk
