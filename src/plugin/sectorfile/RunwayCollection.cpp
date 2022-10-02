#include "sectorfile/RunwayCollection.h"
#include "euroscope/EuroscopeSectorFileElementInterface.h"

using UKControllerPlugin::SectorFile::Runway;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface;
using UKControllerPlugin::SectorFile::SectorFileProviderInterface;

namespace UKControllerPlugin {
    namespace SectorFile {



        RunwayCollection::RunwayCollection(SectorFileProviderInterface & sectorFile)
            : sectorFile(sectorFile)
        {

        }

        size_t RunwayCollection::Count(void) const
        {
            return this->runways.size();
        }

        /*
            Find a runway by identifier and airfield
        */
        const Runway & RunwayCollection::FetchByIdentifierAndAirfield(
            std::string identifier,
            std::string airfield
        ) const {
            std::shared_ptr<Runway> runway = this->GetRunway(this->MakeRunwayKey(airfield, identifier));
            return runway == nullptr ? this->invalidRunway : *runway;
        }

        /*
            When the ASR is loaded, the SectorFile is available. Load the runway data.
        */
        void UKControllerPlugin::SectorFile::RunwayCollection::AsrLoadedEvent(UserSetting & userSetting)
        {
            std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>> runwayElements =
                this->sectorFile.GetAllElementsByType(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);

            for (
                std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>>::const_iterator
                it = runwayElements.cbegin();
                it != runwayElements.cend();
                ++it
            ) {

                std::string airfieldIcao = this->ParseIcaoFromAirfield((*it)->Airport());

                // Skip the default adjacent airports one
                if (airfieldIcao == "") {
                    continue;
                }

                // Process the first of two runways, creating it if not exists or updating if it does
                std::string runway1Key = MakeRunwayKey(airfieldIcao, (*it)->Runway1Identifier());
                std::shared_ptr<Runway> runway1 = this->GetRunway(runway1Key);
                if (!runway1) {
                    runway1 = std::make_shared<Runway>(
                        airfieldIcao,
                        (*it)->Runway1Identifier(),
                        (*it)->Runway1Heading(),
                        (*it)->Runway1ActiveForDepartures(),
                        (*it)->Runway1ActiveForArrivals()
                    );
                    this->runways[runway1Key] = runway1;
                } else {
                    runway1->SetActiveForDepartures((*it)->Runway1ActiveForDepartures());
                    runway1->SetActiveForArrivals((*it)->Runway1ActiveForArrivals());
                }

                // Process the second of two runways, creating it if not exists or updating if it does
                std::string runway2Key = MakeRunwayKey(airfieldIcao, (*it)->Runway2Identifier());
                std::shared_ptr<Runway> runway2 = this->GetRunway(runway2Key);
                if (!runway2) {
                    runway2 = std::make_shared<Runway>(
                        airfieldIcao,
                        (*it)->Runway2Identifier(),
                        (*it)->Runway2Heading(),
                        (*it)->Runway2ActiveForDepartures(),
                        (*it)->Runway2ActiveForArrivals()
                        );
                    this->runways[runway2Key] = runway2;
                } else {
                    runway2->SetActiveForDepartures((*it)->Runway2ActiveForDepartures());
                    runway2->SetActiveForArrivals((*it)->Runway2ActiveForArrivals());
                }
            }
        }

        /*
            Nothing to do when the ASR closes
        */
        void UKControllerPlugin::SectorFile::RunwayCollection::AsrClosingEvent(UserSetting & userSetting)
        {

        }

        /*
            Runway dialog is saved, update the active runways.
        */
        void UKControllerPlugin::SectorFile::RunwayCollection::RunwayDialogSaved(void)
        {
            std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>> runwayElements =
                this->sectorFile.GetAllElementsByType(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);

            for (
                std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>>::const_iterator
                it = runwayElements.begin();
                it != runwayElements.end();
                ++it
            ) {

                std::string airfieldIcao = this->ParseIcaoFromAirfield((*it)->Airport());

                // Skip the default adjacent airports one
                if (airfieldIcao == "") {
                    continue;
                }

                std::shared_ptr<Runway> runway1 = this->GetRunway(
                    this->MakeRunwayKey(airfieldIcao, (*it)->Runway1Identifier())
                );
                std::shared_ptr<Runway> runway2 = this->GetRunway(
                    this->MakeRunwayKey(airfieldIcao, (*it)->Runway2Identifier())
                );

                if (runway1 != nullptr) {
                    runway1->SetActiveForArrivals((*it)->Runway1ActiveForArrivals());
                    runway1->SetActiveForDepartures((*it)->Runway1ActiveForDepartures());
                }

                if (runway2 != nullptr) {
                    runway2->SetActiveForArrivals((*it)->Runway2ActiveForArrivals());
                    runway2->SetActiveForDepartures((*it)->Runway2ActiveForDepartures());
                }
            }
        }

        /*
            Makes the internal storage key for the runway.
        */
        std::string RunwayCollection::MakeRunwayKey(std::string airfield, std::string identifier) const
        {
            return airfield + "." + identifier;
        }

        /*
            Get the ICAO from the airfield name
        */
        std::string RunwayCollection::ParseIcaoFromAirfield(std::string name) const
        {
            std::regex icaoRegex("^([A-Z]{4}) .*$");
            std::smatch matches;

            return std::regex_search(name, matches, icaoRegex) ? std::string(matches[1]) : "";
        }

        /*
            Get a runway from the runways map
        */
        std::shared_ptr<Runway> RunwayCollection::GetRunway(std::string identifier) const
        {
            return this->runways.count(identifier) ? this->runways.at(identifier) : nullptr;
        }
    }  // namespace SectorFile
}  // namespace UKControllerPlugin
