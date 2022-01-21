#include "PairedAirfieldPrenote.h"
#include "PublishedPrenote.h"
#include "PublishedPrenoteCollection.h"
#include "PublishedPrenoteMapper.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "flightrule/FlightRule.h"
#include "flightrule/FlightRuleCollection.h"
#include "sid/SidMapperInterface.h"
#include "sid/StandardInstrumentDeparture.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;

namespace UKControllerPlugin::Prenote {
    PublishedPrenoteMapper::PublishedPrenoteMapper(
        const PublishedPrenoteCollection& publishedPrenotes,
        const Airfield::AirfieldCollection& airfields,
        const Sid::SidMapperInterface& sidMapper,
        const FlightRules::FlightRuleCollection& flightRules)
        : publishedPrenotes(publishedPrenotes), airfields(airfields), sidMapper(sidMapper), flightRules(flightRules)
    {
    }

    /**
     * For a flightplan, find all the airfield pairing and sid published prenotes. Remove duplicates.
     */
    auto PublishedPrenoteMapper::MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::set<std::shared_ptr<PublishedPrenote>>
    {
        std::set<std::shared_ptr<PublishedPrenote>> prenotes;
        this->MapAirfieldPairingPrenotes(flightplan, prenotes);
        this->MapSidPrenotes(flightplan, prenotes);

        return prenotes;
    }

    /**
     * Look at the airfield pairings and find the prenotes that apply.
     */
    void PublishedPrenoteMapper::MapAirfieldPairingPrenotes(
        const EuroScopeCFlightPlanInterface& flightplan, std::set<std::shared_ptr<PublishedPrenote>>& prenotes) const
    {
        const auto departureAirfield = this->airfields.FetchAirfieldByIcao(flightplan.GetOrigin());
        const auto arrivalAirfield = this->airfields.FetchAirfieldByIcao(flightplan.GetDestination());
        const auto flightRules = this->flightRules.GetByEuroscopeKey(flightplan.GetFlightRules());

        if (departureAirfield == nullptr || arrivalAirfield == nullptr || flightRules == nullptr) {
            return;
        }

        for (const auto& airfieldPairing : departureAirfield->AirfieldPairingPrenotes()) {
            auto prenote = this->publishedPrenotes.Get(airfieldPairing->prenoteId);
            if (airfieldPairing->targetAirfield == arrivalAirfield->Id() &&
                airfieldPairing->flightRuleId == flightRules->id && prenote != nullptr) {
                prenotes.insert(prenote);
            }
        }
    }

    /**
     * Look at the SID and get the published prenotes.
     */
    void PublishedPrenoteMapper::MapSidPrenotes(
        const EuroScopeCFlightPlanInterface& flightplan, std::set<std::shared_ptr<PublishedPrenote>>& prenotes) const
    {
        const auto sid = this->sidMapper.MapFlightplanToSid(flightplan);
        if (sid == nullptr) {
            return;
        }

        for (const auto& prenote : sid->Prenotes()) {
            auto publishedPrenote = this->publishedPrenotes.Get(prenote);
            if (publishedPrenote != nullptr) {
                prenotes.insert(publishedPrenote);
            }
        }
    }
} // namespace UKControllerPlugin::Prenote
