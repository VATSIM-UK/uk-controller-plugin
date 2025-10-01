#pragma once

namespace UKControllerPlugin {
    namespace Approach {
        class ApproachModuleFactory;
    } // namespace Approach
    // namespace ECFMP {  // ECFMP temporarily disabled
    //     class ECFMPModuleFactory;
    // } // namespace ECFMP
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
        // [[nodiscard]] auto ECFMP() -> ECFMP::ECFMPModuleFactory&;  // ECFMP temporarily disabled
        [[nodiscard]] auto IntentionCode() -> IntentionCode::IntentionCodeModuleFactory&;

        private:
        // The approach module
        std::unique_ptr<Approach::ApproachModuleFactory> approach;

        // std::unique_ptr<ECFMP::ECFMPModuleFactory> ecfmp;  // ECFMP temporarily disabled

        // The intention code  module
        std::unique_ptr<IntentionCode::IntentionCodeModuleFactory> intentionCode;
    };
} // namespace UKControllerPlugin::Bootstrap
