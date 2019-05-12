#pragma once

namespace UKControllerPlugin {
    namespace Hold {
        struct HoldProfile;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            For comparing hold profiles by their 
            profile name so they can be ordered in structs.
        */
        typedef struct CompareHoldProfile
        {
            using is_transparent = int;

            bool operator()(
                const UKControllerPlugin::Hold::HoldProfile & hold,
                unsigned int id
            ) const;
            bool operator()(
                unsigned int id,
                const UKControllerPlugin::Hold::HoldProfile & hold
            ) const;
            bool operator()(
                const UKControllerPlugin::Hold::HoldProfile & a,
                const UKControllerPlugin::Hold::HoldProfile & b
            ) const;

        } CompareHoldProfile;
    }  // namespace Hold
}  // namespace UKControllerPlugin
