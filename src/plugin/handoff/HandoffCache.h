#pragma once

namespace UKControllerPlugin::Handoff {
    struct ResolvedHandoff;

    /*
     * Contains all the handoffs that have been generated.
     */
    class HandoffCache
    {
        public:
        void Add(const std::shared_ptr<ResolvedHandoff>& handoff);
        void Clear();
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto Get(const std::string& callsign) const -> std::shared_ptr<ResolvedHandoff>;
        void Delete(const std::string& callsign);
        void DeleteWhere(const std::function<bool(const std::shared_ptr<ResolvedHandoff> handoff)>& predicate);

        private:
        // All the handoffs
        std::map<std::string, std::shared_ptr<ResolvedHandoff>> handoffs;
    };
} // namespace UKControllerPlugin::Handoff
