#include "ApproachModuleFactory.h"
#include "ApproachSequencer.h"
#include "ApproachSequencerOptions.h"
#include "ApproachSpacingCalculator.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"

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

    auto ApproachModuleFactory::SequencerOptions() -> ApproachSequencerOptions&
    {
        if (!this->sequencerOptions) {
            this->sequencerOptions = std::make_unique<ApproachSequencerOptions>();
        }

        return *this->sequencerOptions;
    }

    auto ApproachModuleFactory::SpacingCalculator(const Bootstrap::PersistenceContainer& container)
        -> ApproachSpacingCalculator&
    {
        if (!this->spacingCalculator) {
            assert(container.airfields && "Airfield collection is not set on container");
            assert(container.wakeCategoryMappers && "Wake category mappers is not set on container");
            assert(container.plugin && "Plugin is not set on container");
            this->spacingCalculator = std::make_unique<ApproachSpacingCalculator>(
                SequencerOptions(), *container.airfields, *container.wakeCategoryMappers, *container.plugin);
        }

        return *this->spacingCalculator;
    }
} // namespace UKControllerPlugin::Approach
