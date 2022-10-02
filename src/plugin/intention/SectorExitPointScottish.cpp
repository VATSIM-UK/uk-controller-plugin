#include "intention/SectorExitPointScottish.h"

namespace UKControllerPlugin {
    namespace IntentionCode {

        SectorExitPointScottish::SectorExitPointScottish(
            std::string name, std::string intentionCode, unsigned int outDirection)
            : SectorExitPoint(name, intentionCode, outDirection)
        {
        }

        bool SectorExitPointScottish::AppliesToController(std::string callsign) const
        {
            std::string prefix = callsign.substr(0, 3);
            return prefix == this->SCAC_CALLSIGN_PREFIX || prefix == this->SCTC_CALLSIGN_PREFIX ||
                   prefix == this->EGPX_CALLSIGN_PREFIX || prefix == this->EGPX_NI_CALLSIGN_PREFIX ||
                   prefix == this->EGPX_MIL_CALLSIGN_PREFIX;
        }
    } // namespace IntentionCode
} // namespace UKControllerPlugin
