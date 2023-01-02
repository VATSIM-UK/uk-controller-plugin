#pragma once

namespace UKControllerPlugin {
    namespace Approach {
        class ApproachModuleFactory;
    } // namespace Approach
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
        [[nodiscard]] auto IntentionCode() -> IntentionCode::IntentionCodeModuleFactory&;

        private:
        // The approach module
        std::unique_ptr<Approach::ApproachModuleFactory> approach;

        // The intention code  module
        std::unique_ptr<IntentionCode::IntentionCodeModuleFactory> intentionCode;
    };
} // namespace UKControllerPlugin::Bootstrap
