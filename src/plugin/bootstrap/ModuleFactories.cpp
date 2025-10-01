#include "ModuleFactories.h"
#include "approach/ApproachModuleFactory.h"
// #include "ecfmp/ECFMPModuleFactory.h"  // ECFMP temporarily disabled
#include "intention/IntentionCodeModuleFactory.h"

namespace UKControllerPlugin::Bootstrap {

    ModuleFactories::ModuleFactories() = default;
    ModuleFactories::~ModuleFactories() = default;

    auto ModuleFactories::Approach() -> Approach::ApproachModuleFactory&
    {
        if (!approach) {
            approach = std::make_unique<Approach::ApproachModuleFactory>();
        }

        return *approach;
    }

    auto ModuleFactories::IntentionCode() -> IntentionCode::IntentionCodeModuleFactory&
    {
        if (!intentionCode) {
            intentionCode = std::make_unique<IntentionCode::IntentionCodeModuleFactory>();
        }

        return *intentionCode;
    }

    // ECFMP temporarily disabled - requires C++17 compatibility fixes
    // auto ModuleFactories::ECFMP() -> ECFMP::ECFMPModuleFactory&
    // {
    //     if (!ecfmp) {
    //         ecfmp = std::make_unique<ECFMP::ECFMPModuleFactory>();
    //     }
    //
    //     return *ecfmp;
    // }
} // namespace UKControllerPlugin::Bootstrap
