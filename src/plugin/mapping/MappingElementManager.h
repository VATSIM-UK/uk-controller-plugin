#pragma once

namespace UKControllerPlugin::Mapping {
    class MappingElement;

    /**
     * Looks after all the mapping elements.
     */
    class MappingElementManager
    {
        public:
        void Add(std::shared_ptr<MappingElement> element);
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto GetByTypeAndId(const std::string& type, int id) const -> std::shared_ptr<MappingElement>;

        private:
        // Active elements - those being drawn
        std::set<std::shared_ptr<MappingElement>> activeElements;

        std::set<std::shared_ptr<MappingElement>> allElements;
    };
} // namespace UKControllerPlugin::Mapping
