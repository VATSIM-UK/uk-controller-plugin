#pragma once

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Stores information about how a hold is deemed separated from another
        */
        typedef struct DeemedSeparatedHold
        {
            DeemedSeparatedHold(unsigned int identifier, unsigned int vslInsertDistance)
                : identifier(identifier), vslInsertDistance(vslInsertDistance){};

            // The id for the hold
            unsigned int identifier;

            // The distance at which aircraft should be inserted into the VSL
            unsigned int vslInsertDistance;

            /*
                Compare two holds
            */
            bool operator==(const DeemedSeparatedHold& compare) const
            {
                return this->identifier == compare.identifier;
            }
        } DeemedSeparatedHold;
    } // namespace Hold
} // namespace UKControllerPlugin
