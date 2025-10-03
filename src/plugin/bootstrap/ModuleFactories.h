#pragma once

namespace UKControllerPlugin {
    namespace Approach {
        class ApproachModuleFactory;
    } // namespace Approach
#ifdef ENABLE_ECFMP
    namespace ECFMP {
        class ECFMPModuleFactory;
    } // namespace ECFMP
#endif
    namespace IntentionCode {
        class IntentionCodeModuleFactory;
    } // namespace IntentionCode
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Bootstrap {
    /**
     * Contains all the module factory classes so that the objects they store
     * can be accessed.
     *
     * Is not involved in bootstrapping a module with the plugin itself, just storing objects that
     * need to be more widely accessible.
     */
    class ModuleFactories
    {
        public:
        ModuleFactories();
        ~ModuleFactories();
        [[nodiscard]] auto Approach() -> Approach::ApproachModuleFactory&;
#ifdef ENABLE_ECFMP
        [[nodiscard]] auto ECFMP() -> ECFMP::ECFMPModuleFactory&;
#endif
        [[nodiscard]] auto IntentionCode() -> IntentionCode::IntentionCodeModuleFactory&;

        private:
        // The approach module
        std::unique_ptr<Approach::ApproachModuleFactory> approach;

#ifdef ENABLE_ECFMP
        std::unique_ptr<ECFMP::ECFMPModuleFactory> ecfmp;
#endif

        // The intention code  module
        std::unique_ptr<IntentionCode::IntentionCodeModuleFactory> intentionCode;
    };
} // namespace UKControllerPlugin::Bootstrap
