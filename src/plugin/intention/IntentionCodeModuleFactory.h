#pragma once

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin

namespace UKControllerPlugin::IntentionCode {

    class AircraftFirExitGenerator;
    class CachedAircraftFirExitGenerator;
    class FirExitPointCollection;
    class IntentionCodeCollection;

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
        [[nodiscard]] auto IntentionCodes(
            Dependency::DependencyLoaderInterface& dependencyLoader,
            const Controller::ActiveCallsignCollection& activeControllers) -> const IntentionCodeCollection&;

        private:
        // Generates FIR exit points for an aircraft, whilst using a cache
        std::shared_ptr<CachedAircraftFirExitGenerator> firExitGenerator;

        // Stores the FIR exit points
        std::unique_ptr<FirExitPointCollection> exitPoints;

        // Stores the intention codes
        std::unique_ptr<IntentionCodeCollection> intentionCodes;
    };

} // namespace UKControllerPlugin::IntentionCode
