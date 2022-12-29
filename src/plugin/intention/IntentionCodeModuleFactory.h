#pragma once

namespace UKControllerPlugin::Dependency {
    class DependencyLoaderInterface;
} // namespace UKControllerPlugin::Dependency

namespace UKControllerPlugin::IntentionCode {

    class AircraftFirExitGenerator;
    class CachedAircraftFirExitGenerator;
    class FirExitPointCollection;

    /**
     * A class that owns all the bits to do with the intention code module.
     */
    class IntentionCodeModuleFactory
    {
        public:
        IntentionCodeModuleFactory();
        ~IntentionCodeModuleFactory();
        [[nodiscard]] auto FirExitGenerator(Dependency::DependencyLoaderInterface& dependencyLoader)
            -> AircraftFirExitGenerator&;
        [[nodiscard]] auto CachedFirExitGenerator(Dependency::DependencyLoaderInterface& dependencyLoader)
            -> std::shared_ptr<CachedAircraftFirExitGenerator>;
        [[nodiscard]] auto ExitPointCollection(Dependency::DependencyLoaderInterface& dependencyLoader)
            -> const FirExitPointCollection&;

        private:
        // Generates FIR exit points for an aircraft, whilst using a cache
        std::shared_ptr<CachedAircraftFirExitGenerator> firExitGenerator;

        // Stores the FIR exit points
        std::unique_ptr<FirExitPointCollection> exitPoints;
    };

} // namespace UKControllerPlugin::IntentionCode
