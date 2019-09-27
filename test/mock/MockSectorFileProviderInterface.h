#pragma once
#include "pch/pch.h"
#include "sectorfile/SectorFileProviderInterface.h"
#include "euroscope/EuroscopeSectorFileElementInterface.h"

namespace UKControllerPluginTest {
    namespace SectorFile {

        class MockSectorFileProviderInterface :
            public UKControllerPlugin::SectorFile::SectorFileProviderInterface
        {
            public:
                MOCK_METHOD1(
                    GetAllElementsByType,
                    std::set<std::shared_ptr<UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface>>(int)
                );
                MOCK_METHOD0(GetSectorFileName, std::string(void));
        };
    }  // namespace SectorFile
}  // namespace UKControllerPluginTest
