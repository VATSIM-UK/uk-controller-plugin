#pragma once
#include "pch/pch.h"
#include "sectorfile/SectorFileAwareInterface.h"
#include "sectorfile/SectorFileProviderInterface.h"

namespace UKControllerPluginTest {
    namespace SectorFile {

        class MockSectorFileAwareInterface :
            public UKControllerPlugin::SectorFile::SectorFileAwareInterface
        {
            public:
                MOCK_METHOD1(
                    SectorFileUpdate,
                    void(UKControllerPlugin::SectorFile::SectorFileProviderInterface & provider)
                );
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
