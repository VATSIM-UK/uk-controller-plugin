#pragma once

namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyCache;
    }  // namespace Dependency
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Airfield {

        // Forward Declarations
        class Airfield;
        class AirfieldCollection;
        // END

        /*
            A class for creating a collection of airfields based on the dependency
            data downloaded from the webserver.
        */
        class AirfieldCollectionFactory
        {
            public:
                static std::unique_ptr<const UKControllerPlugin::Airfield::AirfieldCollection> Create(
                    const UKControllerPlugin::Dependency::DependencyCache & dependency
                );
                static const std::string requiredDependency;
        };
    }  // namespace Airfield
}  // namespace UKControllerPlugin
