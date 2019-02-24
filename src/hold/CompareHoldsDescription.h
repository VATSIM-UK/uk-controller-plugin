#pragma once

namespace UKControllerPlugin {
    namespace Hold {
        struct HoldingData;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Used for comparing Hold scructs by their description
            field. This is because the description is used when
            ordering holds in a list.
        */
        typedef struct CompareHoldsDescription
        {
            using is_transparent = std::string;

            bool operator()(
                const UKControllerPlugin::Hold::HoldingData & hold,
                std::string description
            ) const;
            bool operator()(
                std::string description,
                const UKControllerPlugin::Hold::HoldingData & hold
            ) const;
            bool operator()(
                const UKControllerPlugin::Hold::HoldingData & a,
                const UKControllerPlugin::Hold::HoldingData & b
            ) const;

        } CompareHolds;
    }  // namespace Hold
}  // namespace UKControllerPlugin
