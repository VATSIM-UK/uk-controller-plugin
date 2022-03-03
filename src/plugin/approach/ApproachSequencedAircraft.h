#pragma once
#include "ApproachSequencingMode.h"

namespace UKControllerPlugin::Approach {

    /**
     * An aircraft that is part of the approach sequencer. When we're working with these, we need to know
     * who comes before and after so this has links to the next in sequence.
     */
    class ApproachSequencedAircraft
    {
        public:
        ApproachSequencedAircraft(std::string callsign, ApproachSequencingMode mode);

        private:
        // Who is the sequenced aircraft
        std::string callsign;

        // What mode are we using in the sequence
        ApproachSequencingMode mode;

        public:
        [[nodiscard]] auto Callsign() const -> const std::string&;
        [[nodiscard]] auto Mode() const -> ApproachSequencingMode;
        void Mode(ApproachSequencingMode mode);
        double ExpectedDistance() const;
        void ExpectedDistance(double expectedDistance);
        [[nodiscard]] auto Previous() const -> const std::shared_ptr<ApproachSequencedAircraft>&;
        void Previous(const std::shared_ptr<ApproachSequencedAircraft>& previous);
        [[nodiscard]] auto Next() const -> const std::shared_ptr<ApproachSequencedAircraft>&;
        void Next(const std::shared_ptr<ApproachSequencedAircraft>& next);

        private:
        // If we're in distance mode, what distance
        double expectedDistance = 6.0;

        // Who is the previous sequenced aircraft
        std::shared_ptr<ApproachSequencedAircraft> previous;

        // Who is the next sequenced aircraft
        std::shared_ptr<ApproachSequencedAircraft> next;
    };
} // namespace UKControllerPlugin::Approach
