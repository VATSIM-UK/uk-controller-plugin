#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace SectorFile {
        EuroScopePlugIn::CPosition GetInvalidPosition(void);
        EuroScopePlugIn::CPosition ParseSectorFileCoordinates(std::string latitude, std::string longitude);
        bool PositionIsInvalid(EuroScopePlugIn::CPosition pos);
    }  // namespace SectorFile
}  // namespace UKControllerPlugin
