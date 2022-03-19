#pragma once
#include "ApproachSequencingMode.h"

namespace UKControllerPlugin::Approach {
    class ApproachSequence;

    /**
     * Handles the approach sequences for each of the airfields.
     */
    class ApproachSequencer
    {
        public:
        void
        AddAircraftToSequence(const std::string& airfield, const std::string& callsign, ApproachSequencingMode mode);
        void AddAircraftToSequence(
            const std::string& airfield,
            const std::string& callsign,
            ApproachSequencingMode mode,
            const std::string& insertBefore);
        void MoveAircraftUp(const std::string& airfield, const std::string& callsign);
        void MoveAircraftDown(const std::string& airfield, const std::string& callsign);
        [[nodiscard]] auto GetForAirfield(const std::string& airfield) -> const ApproachSequence&;
        void RemoveAircraftFromSequences(const std::string& callsign);
        [[nodiscard]] auto CountSequences() const -> size_t;
        void ForEach(const std::function<void(const ApproachSequence&)>& callback);
        [[nodiscard]] auto AirfieldForAircraft(const std::string& callsign) const -> std::string;

        private:
        void AssertAircraftNotInSequence(const std::string& callsign);
        [[nodiscard]] auto GetOrCreate(const std::string& airfield) -> ApproachSequence&;

        std::map<std::string, std::shared_ptr<ApproachSequence>> sequences;
    };
} // namespace UKControllerPlugin::Approach
