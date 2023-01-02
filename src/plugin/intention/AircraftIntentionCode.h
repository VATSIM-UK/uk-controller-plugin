#pragma once

namespace UKControllerPlugin::IntentionCode {
    class IntentionCodeModel;

    /*
        Represents the point(s) at which an aircraft leaves the FIR(s).
    */
    using AircraftIntentionCode = struct AircraftIntentionCode
    {
        // The callsign to which the code belongs
        std::string callsign;

        // The matched intention code string for quick accesss
        std::string intentionCode;

        // The matched intention code model in full
        std::shared_ptr<IntentionCodeModel> matchedIntentionCode;
    };
} // namespace UKControllerPlugin::IntentionCode
