#pragma once

namespace UKControllerPlugin {
    namespace Hold {
        struct HoldingAircraft;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Used for comparing InitialAltitude structs so that they can
            be accessed by string values.
        */
        typedef struct CompareHoldingAircraft
        {
            using is_transparent = std::string;

            bool operator()(
                const UKControllerPlugin::Hold::HoldingAircraft & hold,
                std::string id
            ) const;
            bool operator()(
                std::string id,
                const UKControllerPlugin::Hold::HoldingAircraft & hold
            ) const;
            bool operator()(
                const UKControllerPlugin::Hold::HoldingAircraft & a,
                const UKControllerPlugin::Hold::HoldingAircraft & b
            ) const;

        } CompareHolds;
    }  // namespace Hold
}  // namespace UKControllerPlugin
