#pragma once

namespace UKControllerPlugin {
    namespace Hold {
        struct HoldingData;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Used for comparing InitialAltitude structs so that they can
            be accessed by string values.
        */
        typedef struct CompareHolds
        {
            using is_transparent = std::string;

            bool operator()(
                const UKControllerPlugin::Hold::HoldingData & initial,
                std::string id
            ) const;
            bool operator()(
                std::string id,
                const UKControllerPlugin::Hold::HoldingData & initial
            ) const;
            bool operator()(
                const UKControllerPlugin::Hold::HoldingData & a,
                const UKControllerPlugin::Hold::HoldingData & b
            ) const;

        } CompareHolds;
    }  // namespace Hold
}  // namespace UKControllerPlugin
