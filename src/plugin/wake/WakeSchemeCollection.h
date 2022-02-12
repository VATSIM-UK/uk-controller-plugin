#pragma once

#include "WakeSchemeCollectionFactory.h"

namespace UKControllerPlugin::Wake {
    class WakeScheme;

    class WakeSchemeCollection
    {
        public:
        void Add(std::shared_ptr<WakeScheme> scheme);
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto GetByKey(const std::string& key) const -> std::shared_ptr<WakeScheme>;
        [[nodiscard]] auto GetByName(const std::string& name) const -> std::shared_ptr<WakeScheme>;
        [[nodiscard]] auto FirstWhere(const std::function<bool(const WakeScheme&)> predicate) const
            -> std::shared_ptr<WakeScheme>;
        void ForEach(const std::function<void(const WakeScheme&)> callback) const;

        private:
        std::map<int, std::shared_ptr<WakeScheme>> schemes;
    };
} // namespace UKControllerPlugin::Wake
