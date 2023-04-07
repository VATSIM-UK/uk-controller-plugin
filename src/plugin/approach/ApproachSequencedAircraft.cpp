#include "ApproachSequencedAircraft.h"

namespace UKControllerPlugin::Approach {

    ApproachSequencedAircraft::ApproachSequencedAircraft(std::string callsign, ApproachSequencingMode mode)
        : callsign(std::move(callsign)), mode(mode)
    {
    }

    auto ApproachSequencedAircraft::Mode() const -> ApproachSequencingMode
    {
        return mode;
    }

    void ApproachSequencedAircraft::Mode(ApproachSequencingMode mode)
    {
        this->mode = mode;
    }

    auto ApproachSequencedAircraft::ExpectedDistance() const -> double
    {
        return expectedDistance;
    }

    void ApproachSequencedAircraft::ExpectedDistance(double expectedDistance)
    {
        this->expectedDistance = expectedDistance;
    }

    auto ApproachSequencedAircraft::Previous() const -> const std::shared_ptr<ApproachSequencedAircraft>&
    {
        return previous;
    }

    void ApproachSequencedAircraft::Previous(const std::shared_ptr<ApproachSequencedAircraft>& previous)
    {
        this->previous = previous;
    }

    auto ApproachSequencedAircraft::Next() const -> const std::shared_ptr<ApproachSequencedAircraft>&
    {
        return next;
    }

    void ApproachSequencedAircraft::Next(const std::shared_ptr<ApproachSequencedAircraft>& next)
    {
        this->next = next;
    }

    auto ApproachSequencedAircraft::Callsign() const -> const std::string&
    {
        return callsign;
    }

    auto ApproachSequencedAircraft::ShouldDraw() const -> bool
    {
        return shouldDraw;
    }

    void ApproachSequencedAircraft::ToggleDraw()
    {
        shouldDraw = !shouldDraw;
    }
} // namespace UKControllerPlugin::Approach
