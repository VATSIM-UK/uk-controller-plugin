#include "ApproachSequencerDisplayOptions.h"
#include "ToggleApproachSequencerDisplay.h"

namespace UKControllerPlugin::Approach {

    ToggleApproachSequencerDisplay::ToggleApproachSequencerDisplay(
        std::shared_ptr<ApproachSequencerDisplayOptions> options)
        : options(options)
    {
    }

    std::string ToggleApproachSequencerDisplay::MenuItem() const
    {
        return "Show Approach Sequencer";
    }

    bool ToggleApproachSequencerDisplay::IsVisible() const
    {
        return this->options->IsVisible();
    }

    void ToggleApproachSequencerDisplay::Toggle()
    {
        this->options->ToggleVisible();
    }
} // namespace UKControllerPlugin::Approach
