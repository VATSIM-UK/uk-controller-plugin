#pragma once
#include "euroscope/EuroscopeSectorFileElementInterface.h"

namespace UKControllerPlugin {
    namespace SectorFile {


        /*
            An interface for classes that provide access to EuroScope sector files
        */
        class SectorFileProviderInterface
        {
            public:
                virtual std::set<std::shared_ptr<UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface>> 
                    GetAllElementsByType(int type) = 0;
                virtual std::string GetSectorFileName(void) = 0;
        };
    }  // naemspace Euroscope
}  // namespace UKControllerPlugin
