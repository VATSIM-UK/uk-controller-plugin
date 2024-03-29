#include "ApproachSequence.h"
#include "ApproachSequencer.h"

namespace UKControllerPlugin::Approach {

    auto ApproachSequencer::GetForAirfield(const std::string& airfield) -> const ApproachSequence&
    {
        return GetOrCreate(airfield);
    }

    auto ApproachSequencer::GetOrCreate(const std::string& airfield) -> ApproachSequence&
    {
        if (sequences.count(airfield) == 0) {
            sequences[airfield] = std::make_shared<ApproachSequence>();
        }

        return *sequences.at(airfield);
    }

    void ApproachSequencer::AddAircraftToSequence(
        const std::string& airfield, const std::string& callsign, ApproachSequencingMode mode)
    {
        RemoveAircraftFromSequences(callsign);
        GetOrCreate(airfield).AddAircraftToSequence(callsign, mode);
    }

    void ApproachSequencer::AddAircraftToSequence(
        const std::string& airfield,
        const std::string& callsign,
        ApproachSequencingMode mode,
        const std::string& insertBefore)
    {
        RemoveAircraftFromSequences(callsign);
        GetOrCreate(airfield).AddAircraftToSequence(callsign, mode, insertBefore);
    }

    void ApproachSequencer::RemoveAircraftFromSequences(const std::string& callsign)
    {
        for (const auto& sequence : sequences) {
            if (sequence.second->Get(callsign)) {
                sequence.second->RemoveAircraft(callsign);
                break;
            }
        }
    }

    void ApproachSequencer::MoveAircraftDown(const std::string& airfield, const std::string& callsign)
    {
        GetOrCreate(airfield).MoveAircraftDown(callsign);
    }

    void ApproachSequencer::MoveAircraftUp(const std::string& airfield, const std::string& callsign)
    {
        GetOrCreate(airfield).MoveAircraftUp(callsign);
    }

    auto ApproachSequencer::CountSequences() const -> size_t
    {
        return sequences.size();
    }

    void ApproachSequencer::ForEach(const std::function<void(const ApproachSequence&)>& callback)
    {
        for (const auto& sequence : sequences) {
            callback(*sequence.second);
        }
    }

    auto ApproachSequencer::AirfieldForAircraft(const std::string& aircraft) const -> std::string
    {
        for (const auto& sequence : sequences) {
            if (sequence.second->Get(aircraft) != nullptr) {
                return sequence.first;
            }
        }

        return "";
    }
} // namespace UKControllerPlugin::Approach
