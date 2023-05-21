#pragma once

namespace UKControllerPluginUtils::Collection {
    template <typename KeyType, typename ValueType> class Collection
    {
        public:
        void Add(std::shared_ptr<ValueType> item);
        [[nodiscard]] auto Get(const KeyType& key) const -> std::shared_ptr<ValueType>;
        [[nodiscard]] auto Get(const std::shared_ptr<ValueType>& item) const -> std::shared_ptr<ValueType>;
        [[nodiscard]] auto Contains(const KeyType& key) const -> bool;
        [[nodiscard]] auto Contains(const std::shared_ptr<ValueType>& item) const -> bool;
        [[nodiscard]] auto Count() const -> size_t;
        void RemoveByKey(const KeyType& key);
        void Remove(const std::shared_ptr<ValueType>& item);

        private:
        [[nodiscard]] auto GetLock() const -> std::lock_guard<std::mutex>
        {
            return std::lock_guard(this->mutex);
        }

        std::map<KeyType, std::shared_ptr<ValueType>> items;

        mutable std::mutex mutex;
    };
} // namespace UKControllerPluginUtils::Collection

#include "Collection.tpp"
