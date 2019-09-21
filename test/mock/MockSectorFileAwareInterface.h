#pragma once
#include "pch/pch.h"
#include "euroscope/SectorFileAwareInterface.h"
#include "euroscope/SectorFileProviderInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockSectorFileAwareInterface :
            public UKControllerPlugin::Euroscope::SectorFileAwareInterface
        {
            public:
                MOCK_METHOD1(
                    SectorFileUpdate,
                    void(UKControllerPlugin::Euroscope::SectorFileProviderInterface & provider)
                );
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
