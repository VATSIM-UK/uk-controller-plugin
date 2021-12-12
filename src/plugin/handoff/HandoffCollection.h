#pragma once

namespace UKControllerPlugin::Handoff {
    struct HandoffOrder;

    /*
        A class that stores all the handoff orders available from the plugin
        and how each SID maps to a handoff order.
    */
    class HandoffCollection
    {
        public:
        void Add(std::shared_ptr<HandoffOrder> order);
        [[nodiscard]] auto Get(int id) const -> const std::shared_ptr<HandoffOrder>;
        [[nodiscard]] auto Count() const -> size_t;

        private:
        const std::shared_ptr<HandoffOrder> noOrder = nullptr;

        std::map<int, std::shared_ptr<HandoffOrder>> orders;
    };
} // namespace UKControllerPlugin::Handoff
