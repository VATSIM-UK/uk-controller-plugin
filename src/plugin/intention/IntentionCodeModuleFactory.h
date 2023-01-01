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
            -> std::shared_ptr<AircraftFirExitGenerator>;
        [[nodiscard]] auto CachedFirExitGenerator(Dependency::DependencyLoaderInterface& dependencyLoader)
            -> std::shared_ptr<CachedAircraftFirExitGenerator>;
        [[nodiscard]] auto ExitPointCollection(Dependency::DependencyLoaderInterface& dependencyLoader)
            -> std::shared_ptr<const FirExitPointCollection>;
        [[nodiscard]] auto IntentionCodes(
            Dependency::DependencyLoaderInterface& dependencyLoader,
            std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers)
            -> std::shared_ptr<const IntentionCodeCollection>;
        [[nodiscard]] auto IntentionCodeEventHandlers() -> std::shared_ptr<IntentionCodeEventHandlerCollection>;
        [[nodiscard]] auto IntentionCodeGenerator(
            Dependency::DependencyLoaderInterface& dependencyLoader,
            std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers)
            -> std::shared_ptr<AircraftIntentionCodeGenerator>;
        [[nodiscard]] auto CachedIntentionCodeGenerator(
            Dependency::DependencyLoaderInterface& dependencyLoader,
            std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers)
            -> std::shared_ptr<CachedAircraftIntentionCodeGenerator>;

        private:
        // Generates FIR exit points for an aircraft, whilst using a cache
        std::shared_ptr<CachedAircraftFirExitGenerator> firExitGenerator;

        // Stores the FIR exit points
        std::shared_ptr<FirExitPointCollection> exitPoints;

        // Stores the intention codes
        std::shared_ptr<IntentionCodeCollection> intentionCodes;

        // Stores handlers for intention codes
        std::shared_ptr<IntentionCodeEventHandlerCollection> intentionCodeEventHandlers;

        // Generates intention codes for aircraft
        std::shared_ptr<CachedAircraftIntentionCodeGenerator> intentionCodeGenerator;
    };

} // namespace UKControllerPlugin::IntentionCode
