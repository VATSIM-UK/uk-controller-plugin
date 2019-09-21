#pragma once
#include "pch/pch.h"
#include "euroscope/SectorFileProviderInterface.h"
#include "euroscope/EuroscopeSectorFileElementInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockSectorFileProviderInterface :
            public UKControllerPlugin::Euroscope::SectorFileProviderInterface
        {
            public:
                MOCK_METHOD1(
                    GetAllElementsByType,
                    std::set<std::shared_ptr<UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface>>(int)
                );
                MOCK_METHOD0(GetSectorFileName, std::string(void));
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
