#pragma once
#include "intention/SectorExitPoint.h"

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            A class that handles Sector Exit Points
            for the EGPX FIR positions
        */
        class SectorExitPointScottish : public SectorExitPoint
        {
            public:
                SectorExitPointScottish(std::string name, std::string intentionCode, unsigned int outDirection);
                bool AppliesToController(std::string callsign) const;

            private:
                std::string currentController;
                const std::string SCAC_CALLSIGN_PREFIX = "SCO";
                const std::string SCTC_CALLSIGN_PREFIX = "STC";
                const std::string EGPX_CALLSIGN_PREFIX = "EGP";
                const std::string EGPX_MIL_CALLSIGN_PREFIX = "EGP";
                const std::string EGPX_NI_CALLSIGN_PREFIX = "EGA";
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
