#include "CachedAircraftFirExitGenerator.h"
#include "CachedAircraftIntentionCodeGenerator.h"
#include "FirExitPointCollection.h"
#include "FirExitPointCollectionFactory.h"
#include "IntentionCodeCollection.h"
#include "IntentionCodeCollectionFactory.h"
#include "IntentionCodeEventHandlerCollection.h"
#include "IntentionCodeModuleFactory.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin::IntentionCode {

    struct IntentionCodeModuleFactory::Impl
    {
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

    IntentionCodeModuleFactory::IntentionCodeModuleFactory()
        : pimpl(std::make_unique<IntentionCodeModuleFactory::Impl>())
    {
    }

    IntentionCodeModuleFactory::~IntentionCodeModuleFactory() = default;

    auto IntentionCodeModuleFactory::FirExitGenerator(Dependency::DependencyLoaderInterface& dependencyLoader)
        -> std::shared_ptr<AircraftFirExitGenerator>
    {
        return CachedFirExitGenerator(dependencyLoader);
    }

    auto IntentionCodeModuleFactory::ExitPointCollection(Dependency::DependencyLoaderInterface& dependencyLoader)
        -> std::shared_ptr<const FirExitPointCollection>
    {
        if (!pimpl->exitPoints) {
            pimpl->exitPoints = MakeFirExitPointCollection(
                dependencyLoader.LoadDependency("DEPENDENCY_FIR_EXIT_POINTS", nlohmann::json::array()));
        }

        return pimpl->exitPoints;
    }

    auto IntentionCodeModuleFactory::CachedFirExitGenerator(Dependency::DependencyLoaderInterface& dependencyLoader)
        -> std::shared_ptr<CachedAircraftFirExitGenerator>
    {
        if (!pimpl->firExitGenerator) {
            pimpl->firExitGenerator =
                std::make_shared<CachedAircraftFirExitGenerator>(ExitPointCollection(dependencyLoader));
        }

        return pimpl->firExitGenerator;
    }

    auto IntentionCodeModuleFactory::IntentionCodes(
        Dependency::DependencyLoaderInterface& dependencyLoader,
        std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers)
        -> std::shared_ptr<const IntentionCodeCollection>
    {
        if (!pimpl->intentionCodes) {
            pimpl->intentionCodes = MakeIntentionCodeCollection(
                dependencyLoader.LoadDependency("DEPENDENCY_INTENTION_CODES", nlohmann::json::array()),
                FirExitGenerator(dependencyLoader),
                activeControllers);
        }

        return pimpl->intentionCodes;
    }

    auto IntentionCodeModuleFactory::IntentionCodeEventHandlers()
        -> std::shared_ptr<IntentionCodeEventHandlerCollection>
    {
        if (!pimpl->intentionCodeEventHandlers) {
            pimpl->intentionCodeEventHandlers = std::make_shared<IntentionCodeEventHandlerCollection>();
        }

        return pimpl->intentionCodeEventHandlers;
    }

    auto IntentionCodeModuleFactory::IntentionCodeGenerator(
        Dependency::DependencyLoaderInterface& dependencyLoader,
        std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers)
        -> std::shared_ptr<AircraftIntentionCodeGenerator>
    {
        return CachedIntentionCodeGenerator(dependencyLoader, activeControllers);
    }

    auto IntentionCodeModuleFactory::CachedIntentionCodeGenerator(
        Dependency::DependencyLoaderInterface& dependencyLoader,
        std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers)
        -> std::shared_ptr<CachedAircraftIntentionCodeGenerator>
    {
        if (!pimpl->intentionCodeGenerator) {
            pimpl->intentionCodeGenerator = std::make_shared<CachedAircraftIntentionCodeGenerator>(
                IntentionCodes(dependencyLoader, activeControllers), IntentionCodeEventHandlers());
        }

        return pimpl->intentionCodeGenerator;
    }
} // namespace UKControllerPlugin::IntentionCode
