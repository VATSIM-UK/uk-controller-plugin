#pragma once
#include "CollectionIterator.h"

namespace UKControllerPluginUtils::Collection {
    template <typename KeyType, typename ValueType> class Collection
    {
        public:
        void Add(std::shared_ptr<ValueType> item);
        [[nodiscard]] auto FirstOrDefault(const std::function<bool(const std::shared_ptr<ValueType>&)>& predicate) const
            -> const std::shared_ptr<ValueType>;
        [[nodiscard]] auto Get(const KeyType& key) const -> std::shared_ptr<ValueType>;
        [[nodiscard]] auto Get(const std::shared_ptr<ValueType>& item) const -> std::shared_ptr<ValueType>;
        [[nodiscard]] auto Contains(const KeyType& key) const -> bool;
        [[nodiscard]] auto Contains(const std::shared_ptr<ValueType>& item) const -> bool;
        [[nodiscard]] auto Count() const -> size_t;
        void RemoveByKey(const KeyType& key);
        void Remove(const std::shared_ptr<ValueType>& item);
        void RemoveWhere(const std::function<bool(const std::shared_ptr<ValueType>&)>& predicate);

        /*
         * Iterators
         */
        [[nodiscard]] auto begin()
            -> CollectionIterator<KeyType, ValueType, typename std::map<KeyType, std::shared_ptr<ValueType>>::iterator>;
        [[nodiscard]] auto end()
            -> CollectionIterator<KeyType, ValueType, typename std::map<KeyType, std::shared_ptr<ValueType>>::iterator>;
        [[nodiscard]] auto cbegin() const -> CollectionIterator<
            KeyType,
            ValueType,
            typename std::map<KeyType, std::shared_ptr<ValueType>>::const_iterator>;
        [[nodiscard]] auto cend() const -> CollectionIterator<
            KeyType,
            ValueType,
            typename std::map<KeyType, std::shared_ptr<ValueType>>::const_iterator>;
        [[nodiscard]] auto rbegin() -> CollectionIterator<
            KeyType,
            ValueType,
            typename std::map<KeyType, std::shared_ptr<ValueType>>::reverse_iterator>;
        [[nodiscard]] auto rend() -> CollectionIterator<
            KeyType,
            ValueType,
            typename std::map<KeyType, std::shared_ptr<ValueType>>::reverse_iterator>;

        private:
        [[nodiscard]] auto GetLock() const -> std::lock_guard<std::recursive_mutex>
        {
            return std::lock_guard(this->mutex);
        }

        std::map<KeyType, std::shared_ptr<ValueType>> items;

        mutable std::recursive_mutex mutex;
    };
} // namespace UKControllerPluginUtils::Collection

#include "Collection.tpp"
