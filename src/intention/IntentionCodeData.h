#pragma once

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Structure containing data about an intention code.
        */
        using IntentionCodeData = struct IntentionCodeData {

            IntentionCodeData(void)
                : intentionCode("--"), exitPointValid(false), exitPointIndex(-1), exitPoint("")
            {

            }

            IntentionCodeData(
                std::string intentionCode,
                bool exitPointValid,
                int exitPointIndex,
                std::string exitPoint
            )
                : intentionCode(intentionCode), exitPointValid(exitPointValid), exitPointIndex(exitPointIndex),
                  exitPoint(std::move(exitPoint))
            {

            };

            // The code
            std::string intentionCode;

            // Whether or not the exit point is a valid one
            bool exitPointValid;

            // The index on the extracted route at which the aircraft leaves the FIR
            int exitPointIndex;

            // The exit point itself
            std::string exitPoint;
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
