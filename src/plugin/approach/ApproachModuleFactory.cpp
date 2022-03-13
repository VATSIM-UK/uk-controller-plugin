#include "ApproachModuleFactory.h"
#include "ApproachSequencer.h"

namespace UKControllerPlugin::Approach {

    ApproachModuleFactory::ApproachModuleFactory() = default;
    ApproachModuleFactory::~ApproachModuleFactory() = default;

    auto ApproachModuleFactory::Sequencer() -> ApproachSequencer&
    {
        if (!this->sequencer) {
            this->sequencer = std::make_unique<ApproachSequencer>();
        }

        return *this->sequencer;
    }
} // namespace UKControllerPlugin::Approach
