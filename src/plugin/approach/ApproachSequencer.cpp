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
        AssertAircraftNotInSequence(callsign);
        GetOrCreate(airfield).AddAircraftToSequence(callsign, mode);
    }

    void ApproachSequencer::AddAircraftToSequence(
        const std::string& airfield,
        const std::string& callsign,
        ApproachSequencingMode mode,
        const std::string& insertBefore)
    {
        AssertAircraftNotInSequence(callsign);
        GetOrCreate(airfield).AddAircraftToSequence(callsign, mode, insertBefore);
    }

    void ApproachSequencer::AssertAircraftNotInSequence(const std::string& callsign)
    {
        for (const auto& sequence : sequences) {
            assert(sequence.second->Get(callsign) == nullptr && "Aircraft already in arrival sequence");
        }
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
} // namespace UKControllerPlugin::Approach
