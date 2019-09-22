#pragma once
#include "pch/stdafx.h"
#include "sectorfile/SectorFileProviderInterface.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        /*
            An interface for classes that want to know
            when something changes to do with the sector file.
        */
        class SectorFileAwareInterface
        {
            public:

                virtual void SectorFileUpdate(
                    UKControllerPlugin::SectorFile::SectorFileProviderInterface & provider
                ) = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
