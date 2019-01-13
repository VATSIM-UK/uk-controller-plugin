#pragma once

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Structure containing data about an intention code.
        */
        typedef struct IntentionCodeData {

            IntentionCodeData(void)
                : intentionCode("--"), exitPointIndex(-1), exitPointValid(false)
            {

            }

            IntentionCodeData(std::string intentionCode, bool exitPointValid, int exitPointIndex)
                : intentionCode(intentionCode), exitPointValid(exitPointValid), exitPointIndex(exitPointIndex)
            {

            };

            // The code
            std::string intentionCode;

            // Whether or not the exit point is a valid one
            bool exitPointValid;

            // The index on the extracted route at which the aircraft leaves the FIR
            int exitPointIndex;
        } IntentionCodeData;
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
