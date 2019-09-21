#pragma once
#include "pch/stdafx.h"
#include "euroscope/SectorFileProviderInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            An interface for classes that want to know
            when something changes to do with the sector file.
        */
        class SectorFileAwareInterface
        {
            public:

                virtual void SectorFileUpdate(
                    UKControllerPlugin::Euroscope::SectorFileProviderInterface & provider
                ) = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
