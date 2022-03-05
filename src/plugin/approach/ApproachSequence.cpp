#include "ApproachSequence.h"
#include "ApproachSequencedAircraft.h"

namespace UKControllerPlugin::Approach {

    void ApproachSequence::AddAircraftToSequence(const std::string& callsign, ApproachSequencingMode mode)
    {
        std::shared_ptr<ApproachSequencedAircraft> lastAircraft = nullptr;

        auto aircraft = std::make_shared<ApproachSequencedAircraft>(callsign, mode);
        if (!sequencedAircraft.empty()) {
            lastAircraft = sequencedAircraft.back();
            lastAircraft->Next(aircraft);
        }

        aircraft->Previous(lastAircraft);
        sequencedAircraft.push_back(aircraft);
    }

    void ApproachSequence::AddAircraftToSequence(
        const std::string& callsign, ApproachSequencingMode mode, const std::string& insertBefore)
    {
        auto existingAircraft = this->AircraftMatchingCallsign(insertBefore);
        if (existingAircraft != sequencedAircraft.cend()) {
            auto existing = *existingAircraft;
            auto aircraft = std::make_shared<ApproachSequencedAircraft>(callsign, mode);

            // If the existing aircraft had a previous, switch it to this new one
            if (existing->Previous()) {
                aircraft->Previous(existing->Previous());
                aircraft->Previous()->Next(aircraft);
            }

            // Link up these two
            aircraft->Next(existing);
            existing->Previous(aircraft);

            sequencedAircraft.insert(existingAircraft, aircraft);
        } else {
            AddAircraftToSequence(callsign, mode);
        }
    }

    void ApproachSequence::RemoveAircraft(const std::string& callsign)
    {
        auto sequenced = this->AircraftMatchingCallsign(callsign);
        if (sequenced != sequencedAircraft.cend()) {
            auto aircraft = *sequenced;
            if (aircraft->Next() && aircraft->Previous()) {
                aircraft->Next()->Previous(aircraft->Previous());
                aircraft->Previous()->Next(aircraft->Next());
            } else if (aircraft->Next()) {
                aircraft->Next()->Previous(nullptr);
            } else if (aircraft->Previous()) {
                aircraft->Previous()->Next(nullptr);
            }

            sequencedAircraft.erase(sequenced);
        }
    }

    auto ApproachSequence::Get(const std::string& callsign) const -> const std::shared_ptr<ApproachSequencedAircraft>
    {
        auto aircraft = this->AircraftMatchingCallsign(callsign);
        return aircraft == this->sequencedAircraft.cend() ? nullptr : *aircraft;
    }

    auto ApproachSequence::First() const -> const std::shared_ptr<ApproachSequencedAircraft>
    {
        return this->sequencedAircraft.empty() ? nullptr : this->sequencedAircraft.front();
    }

    auto ApproachSequence::AircraftMatchingCallsign(const std::string& callsign) const
        -> std::list<std::shared_ptr<ApproachSequencedAircraft>>::const_iterator
    {
        return std::find_if(
            sequencedAircraft.cbegin(),
            sequencedAircraft.cend(),
            [&callsign](const std::shared_ptr<ApproachSequencedAircraft>& sequencedAircraft) -> bool {
                return sequencedAircraft->Callsign() == callsign;
            });
    }
} // namespace UKControllerPlugin::Approach
