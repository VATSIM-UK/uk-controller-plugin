#include "pch/stdafx.h"
#include "sectorfile/SectorFileCoordinates.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        std::regex latitudePattern("^([N,S])(\\d{3})\\.(\\d{2})\\.(\\d{2})\\.(\\d{3})$");
        std::regex longitudePattern("^([E,W])(\\d{3})\\.(\\d{2})\\.(\\d{2})\\.(\\d{3})$");

        EuroScopePlugIn::CPosition GetInvalidPosition(void)
        {
            EuroScopePlugIn::CPosition pos;
            pos.m_Latitude = -9999.0;
            pos.m_Longitude = -9999.0;
            return pos;
        }

        EuroScopePlugIn::CPosition ParseSectorFileCoordinates(std::string latitude, std::string longitude)
        {
            std::match_results<std::string::const_iterator> latitudeMatch;
            std::match_results<std::string::const_iterator> longitudeMatch;

            if (
                !std::regex_match(latitude, latitudeMatch, latitudePattern) ||
                !std::regex_match(longitude, longitudeMatch, longitudePattern)
            ) {
                return GetInvalidPosition();
            }

            // Get the values out of the regex
            int latitudeDegrees = std::stoi(latitudeMatch[2].str());
            int latitudeMinutes = std::stoi(latitudeMatch[3].str());
            double latitudeSeconds = std::stod(latitudeMatch[4].str() + "." + latitudeMatch[5].str());
            int longitudeDegrees = std::stoi(longitudeMatch[2].str());
            int longitudeMinutes = std::stoi(longitudeMatch[3].str());
            double longitudeSeconds = std::stod(longitudeMatch[4].str() + "." + longitudeMatch[5].str());

            // Check the validity of the values
            if (
                (latitudeDegrees == 90 && (latitudeMinutes != 0 || latitudeSeconds != 0)) ||
                latitudeDegrees > 90 ||
                latitudeMinutes >= 60 ||
                latitudeSeconds >= 60.0 ||
                (longitudeDegrees == 180 && (longitudeMinutes != 0 || longitudeSeconds != 0)) ||
                longitudeDegrees > 180 ||
                longitudeMinutes >= 60 ||
                longitudeSeconds >= 60.0
            ) {
                return GetInvalidPosition();
            }

            EuroScopePlugIn::CPosition position;
            position.m_Latitude = latitudeDegrees + (latitudeMinutes / 60.0) + (latitudeSeconds / 3600.0);
            position.m_Longitude = longitudeDegrees + (longitudeMinutes / 60.0) + (longitudeSeconds / 3600.0);

            if (latitudeMatch[1] == "S") {
                position.m_Latitude *= -1;
            }

            if (longitudeMatch[1] == "W") {
                position.m_Longitude *= -1;
            }

            return position;
        }

        bool PositionIsInvalid(EuroScopePlugIn::CPosition pos)
        {
            EuroScopePlugIn::CPosition invalid = GetInvalidPosition();
            return invalid.m_Latitude == pos.m_Latitude &&
                invalid.m_Longitude == pos.m_Longitude;
        }
    }  // namespace SectorFile
}  // namespace UKControllerPlugin
