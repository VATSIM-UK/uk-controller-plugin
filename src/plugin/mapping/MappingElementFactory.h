#pragma once

namespace UKControllerPlugin::Mapping {
    class MappingElementDrawerFactory;
    class MappingElement;

    class MappingElementFactory
    {
        public:
        MappingElementFactory(const MappingElementDrawerFactory& drawerFactory);
        [[nodiscard]] auto Make(const nlohmann::json& data) -> std::shared_ptr<MappingElement>;

        private:
        [[nodiscard]] static auto DataIsValid(const nlohmann::json& data) -> bool;
        [[nodiscard]] static auto IsKnownType(const std::string& type) -> bool;

        // Builds drawers
        const MappingElementDrawerFactory& drawerFactory;
    };
} // namespace UKControllerPlugin::Mapping
