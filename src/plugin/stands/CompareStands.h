#pragma once
#include "stands/Stand.h"

namespace UKControllerPlugin {
    namespace Stands {

        /*
            Used for comparing Stand structs
        */
        typedef struct CompareStands
        {
            using is_transparent = int;

            bool operator()(
                const UKControllerPlugin::Stands::Stand& stand,
                int id
                ) const;
            bool operator()(
                int id,
                const UKControllerPlugin::Stands::Stand& stand
                ) const;
            bool operator()(
                const UKControllerPlugin::Stands::Stand& standA,
                const UKControllerPlugin::Stands::Stand& standB
            ) const;

        } CompareStands;
    }  // namespace Stands
}  // namespace UKControllerPlugin
