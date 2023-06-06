#pragma once

namespace UKControllerPluginUtils::Collection {
    template <typename KeyType, typename ValueType, typename IteratorType> class CollectionIterator
    {
        public:
        CollectionIterator(std::recursive_mutex& lockingMutex, IteratorType current);
        CollectionIterator(const CollectionIterator& old);
        ~CollectionIterator();
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = ValueType;
        using pointer = ValueType*;   // or also value_type*
        using reference = ValueType&; // or also value_type&

        auto operator*() const -> ValueType&;
        auto operator->() const -> ValueType*;
        auto operator++() -> CollectionIterator<KeyType, ValueType, IteratorType>&;
        auto operator++(int) -> CollectionIterator<KeyType, ValueType, IteratorType>;
        auto operator==(const CollectionIterator<KeyType, ValueType, IteratorType>& compare) const -> bool;
        auto operator!=(const CollectionIterator<KeyType, ValueType, IteratorType>& compare) const -> bool;

        private:
        std::unique_lock<std::recursive_mutex> lock;

        // The current iterator position
        IteratorType current;
    };
} // namespace UKControllerPluginUtils::Collection

#include "CollectionIterator.tpp"
