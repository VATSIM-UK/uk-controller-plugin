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
    class AircraftIntentionCodeGenerator;
    class CachedAircraftIntentionCodeGenerator;
    class CachedAircraftFirExitGenerator;
    class FirExitPointCollection;
    class IntentionCodeCollection;
    class IntentionCodeEventHandlerCollection;

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
        [[nodiscard]] auto IntentionCodeEventHandlers() -> IntentionCodeEventHandlerCollection&;
        [[nodiscard]] auto IntentionCodeGenerator(
            Dependency::DependencyLoaderInterface& dependencyLoader,
            const Controller::ActiveCallsignCollection& activeControllers) -> AircraftIntentionCodeGenerator&;
        [[nodiscard]] auto CachedIntentionCodeGenerator(
            Dependency::DependencyLoaderInterface& dependencyLoader,
            const Controller::ActiveCallsignCollection& activeControllers) -> CachedAircraftIntentionCodeGenerator&;

        private:
        // Generates FIR exit points for an aircraft, whilst using a cache
        std::shared_ptr<CachedAircraftFirExitGenerator> firExitGenerator;

        // Stores the FIR exit points
        std::unique_ptr<FirExitPointCollection> exitPoints;

        // Stores the intention codes
        std::unique_ptr<IntentionCodeCollection> intentionCodes;

        // Stores handlers for intention codes
        std::unique_ptr<IntentionCodeEventHandlerCollection> intentionCodeEventHandlers;

        // Generates intention codes for aircraft
        std::unique_ptr<CachedAircraftIntentionCodeGenerator> intentionCodeGenerator;
    };

} // namespace UKControllerPlugin::IntentionCode
