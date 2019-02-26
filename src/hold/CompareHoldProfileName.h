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
        typedef struct CompareHoldProfileName
        {
            using is_transparent = std::string;

            bool operator()(
                const UKControllerPlugin::Hold::HoldProfile & hold,
                std::string name
            ) const;
            bool operator()(
                std::string name,
                const UKControllerPlugin::Hold::HoldProfile & hold
            ) const;
            bool operator()(
                const UKControllerPlugin::Hold::HoldProfile & a,
                const UKControllerPlugin::Hold::HoldProfile & b
            ) const;

        } CompareHolds;
    }  // namespace Hold
}  // namespace UKControllerPlugin
