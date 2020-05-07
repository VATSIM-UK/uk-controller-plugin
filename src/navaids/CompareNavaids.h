#pragma once
#include "navaids/Navaid.h"

namespace UKControllerPlugin {
    namespace Navaids {

        /*
            Used for comparing InitialAltitude structs so that they can
            be accessed by string values.
        */
        typedef struct CompareNavaids
        {
            using is_transparent = std::string;

            bool operator()(
                const UKControllerPlugin::Navaids::Navaid& navaid,
                std::string identifier
            ) const;
            bool operator()(
                std::string identifier,
                const UKControllerPlugin::Navaids::Navaid & navaid
            ) const;
            bool operator()(
                const UKControllerPlugin::Navaids::Navaid & a,
                const UKControllerPlugin::Navaids::Navaid & b
            ) const;

        } CompareNavaids;
    }  // namespace navaids
}  // namespace UKControllerPlugin
