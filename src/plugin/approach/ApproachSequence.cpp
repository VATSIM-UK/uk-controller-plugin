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

    void ApproachSequence::MoveAircraftUp(const std::string& callsign)
    {
        auto aircraftIterator = this->AircraftMatchingCallsign(callsign);
        if (aircraftIterator == this->sequencedAircraft.cend() || (*aircraftIterator)->Previous() == nullptr) {
            return;
        }

        // Get all the iterators
        auto aircraft = *aircraftIterator;
        auto nextAircraftIterator = aircraft->Next() ? this->AircraftMatchingCallsign(aircraft->Next()->Callsign())
                                                     : this->sequencedAircraft.cend();
        auto previousAircraftIterator = aircraft->Previous()
                                            ? this->AircraftMatchingCallsign(aircraft->Previous()->Callsign())
                                            : this->sequencedAircraft.cend();

        // Sort out the relationship between the two aircraft being switched
        auto previousAircraft =
            previousAircraftIterator == sequencedAircraft.cend() ? nullptr : *previousAircraftIterator;
        if (previousAircraft) {
            if (previousAircraft->Previous()) {
                previousAircraft->Previous()->Next(aircraft);
            }

            aircraft->Previous(previousAircraft->Previous());
            previousAircraft->Previous(aircraft);
        }

        aircraft->Next(previousAircraft);

        // Sort out the relationship between the aircraft thats moving down and the "next aircraft"
        auto nextAircraft = nextAircraftIterator != this->sequencedAircraft.cend() ? *nextAircraftIterator : nullptr;
        if (nextAircraft) {
            nextAircraft->Previous(previousAircraft);
        }

        previousAircraft->Next(nextAircraft);

        // Re-sequence the list
        this->sequencedAircraft.erase(aircraftIterator);
        this->sequencedAircraft.insert(previousAircraftIterator, aircraft);
    }

    void ApproachSequence::MoveAircraftDown(const std::string& callsign)
    {
        auto aircraftIterator = this->AircraftMatchingCallsign(callsign);
        if (aircraftIterator == this->sequencedAircraft.cend() || (*aircraftIterator)->Next() == nullptr) {
            return;
        }

        // Get all the iterators
        auto aircraft = *aircraftIterator;
        auto nextAircraftIterator = aircraft->Next() ? this->AircraftMatchingCallsign(aircraft->Next()->Callsign())
                                                     : this->sequencedAircraft.cend();
        auto previousAircraftIterator = aircraft->Previous()
                                            ? this->AircraftMatchingCallsign(aircraft->Previous()->Callsign())
                                            : this->sequencedAircraft.cend();

        // Sort out the relationship between the two aircraft being switched
        auto nextAircraft = *nextAircraftIterator;
        if (nextAircraft) {
            if (nextAircraft->Next()) {
                nextAircraft->Next()->Previous(aircraft);
            }
            aircraft->Next(nextAircraft->Next());
            nextAircraft->Next(aircraft);
        }
        aircraft->Previous(nextAircraft);

        // Sort out the relationship between the aircraft thats moving up and the "previous aircraft"
        auto previousAircraft =
            previousAircraftIterator != this->sequencedAircraft.cend() ? *previousAircraftIterator : nullptr;
        if (previousAircraft) {
            previousAircraft->Next(nextAircraft);
        }
        nextAircraft->Previous(previousAircraft);

        // Re-sequence the list
        this->sequencedAircraft.erase(nextAircraftIterator);
        this->sequencedAircraft.insert(aircraftIterator, nextAircraft);
    }

    auto ApproachSequence::Callsigns() const -> std::list<std::string>
    {
        std::list<std::string> callsigns;
        for (const auto& aircraft : this->sequencedAircraft) {
            callsigns.push_back(aircraft->Callsign());
        }

        return callsigns;
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
