#pragma once
#include "sectorfile/Runway.h"
#include "euroscope/RunwayDialogAwareInterface.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "sectorfile/SectorFileProviderInterface.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        /*
            Collects together all the runways defined in the users sectorfile.
        */
        class RunwayCollection : public UKControllerPlugin::Euroscope::AsrEventHandlerInterface,
                                 public UKControllerPlugin::Euroscope::RunwayDialogAwareInterface
        {
            public:
            RunwayCollection(UKControllerPlugin::SectorFile::SectorFileProviderInterface& sectorFile);
            size_t Count(void) const;
            const UKControllerPlugin::SectorFile::Runway&
            FetchByIdentifierAndAirfield(std::string identifier, std::string airfield) const;

            // Inherited via AsrEventHandlerInterface
            void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
            void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;

            // Inherited via RunwayDialogAwareInterface
            void RunwayDialogSaved(void) override;

            const UKControllerPlugin::SectorFile::Runway invalidRunway = {"", "", 371, false, false};

            private:
            std::shared_ptr<Runway> GetRunway(std::string identifier) const;
            std::string MakeRunwayKey(std::string airfield, std::string identifier) const;
            std::string ParseIcaoFromAirfield(std::string name) const;

            // A map of sector file identifier to runways
            std::map<std::string, std::shared_ptr<UKControllerPlugin::SectorFile::Runway>> runways;

            // Provides sectorfile information
            UKControllerPlugin::SectorFile::SectorFileProviderInterface& sectorFile;
        };
    } // namespace SectorFile
} // namespace UKControllerPlugin
