#include "ApproachSequencerDisplayOptions.h"

namespace UKControllerPlugin::Approach {

    auto ApproachSequencerDisplayOptions::Airfield() const -> const std::string&
    {
        return airfield;
    }

    void ApproachSequencerDisplayOptions::Airfield(const std::string& airfield)
    {
        this->airfield = airfield;
    }

    auto ApproachSequencerDisplayOptions::ContentCollapsed() const -> bool
    {
        return contentCollapsed;
    }

    void ApproachSequencerDisplayOptions::ToggleCollapsed()
    {
        contentCollapsed = !contentCollapsed;
    }

    auto ApproachSequencerDisplayOptions::IsVisible() const -> bool
    {
        return isVisible;
    }

    void ApproachSequencerDisplayOptions::ToggleVisible()
    {
        isVisible = !isVisible;
    }

    auto ApproachSequencerDisplayOptions::Position() const -> const POINT&
    {
        return position;
    }

    void ApproachSequencerDisplayOptions::Position(const POINT& position)
    {
        this->position = position;
    }

    void ApproachSequencerDisplayOptions::SetCollapsed(bool collapsed)
    {
        this->contentCollapsed = collapsed;
    }

    void ApproachSequencerDisplayOptions::SetVisible(bool visible)
    {
        this->isVisible = visible;
    }
} // namespace UKControllerPlugin::Approach
