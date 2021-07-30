#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace Srd {

        /*
            Search parameters when performing an SRD search
        */
        typedef struct SrdSearchParameters
        {
            std::string origin;

            std::string destination;

            unsigned int requestedLevel = NULL;
        } SrdSearchParameters;
    }  // namespace Srd
}  // namespace UKControllerPlugin
