#pragma once
#include "initialaltitude/InitialAltitudeGenerator.h"

// Forward declare
namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyLoaderInterface;
    }  // namespace Dependency
}  // namespace UKControllerPlugin


namespace UKControllerPlugin {
    namespace InitialAltitude {
        class InitialAltitudeGenerator;
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace InitialAltitude {

        /*
            Factory class for creating an InitialAltitudeGenerator object and loading its dependency.
            Only includes IA's from the dependencies that are integers.
        */
        class InitialAltitudeGeneratorFactory
        {
            public:
                static std::unique_ptr<UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator> Create(
                    UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency
                );
        };
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
