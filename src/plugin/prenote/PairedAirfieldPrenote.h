#pragma once

namespace UKControllerPlugin::Prenote {
    using PairedAirfieldPrenote = struct PairedAirfieldPrenote
    {
        PairedAirfieldPrenote(int targetAirfield, int flightRuleId, int prenoteId)
            : targetAirfield(targetAirfield), flightRuleId(flightRuleId), prenoteId(prenoteId)
        {
        
        }
        
        const int targetAirfield;

        const int flightRuleId;

        const int prenoteId;
    };
} // namespace UKControllerPlugin::Prenote
