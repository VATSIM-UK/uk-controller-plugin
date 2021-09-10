#pragma once

namespace UKControllerPlugin::SectorFile {
    [[nodiscard]] auto GetInvalidPosition() -> EuroScopePlugIn::CPosition;
    [[nodiscard]] auto ParseSectorFileCoordinates(std::string latitude, std::string longitude)
        -> EuroScopePlugIn::CPosition;
    [[nodiscard]] auto PositionIsInvalid(EuroScopePlugIn::CPosition pos) -> bool;
} // namespace UKControllerPlugin::SectorFile
