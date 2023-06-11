namespace UKControllerPluginUtils::Collection {
    template <typename KeyType, typename ValueType, typename IteratorType>
    CollectionIterator<KeyType, ValueType, IteratorType>::CollectionIterator(
        std::recursive_mutex& lockingMutex, IteratorType current)
    {
        this->lock = std::unique_lock(lockingMutex);
        this->current = std::move(current);
    }

    template <typename KeyType, typename ValueType, typename IteratorType>
    CollectionIterator<KeyType, ValueType, IteratorType>::CollectionIterator(const CollectionIterator& old)
    {
        this->lock = std::unique_lock(*old.lock.mutex());
        this->current = old.current;
    }

    template <typename KeyType, typename ValueType, typename IteratorType>
    CollectionIterator<KeyType, ValueType, IteratorType>::~CollectionIterator() = default;

    template <typename KeyType, typename ValueType, typename IteratorType>
    auto CollectionIterator<KeyType, ValueType, IteratorType>::operator*() const -> ValueType&
    {
        return *current->second;
    }

    template <typename KeyType, typename ValueType, typename IteratorType>
    auto CollectionIterator<KeyType, ValueType, IteratorType>::operator->() const -> ValueType*
    {
        return current->second.get();
    }

    template <typename KeyType, typename ValueType, typename IteratorType>
    auto CollectionIterator<KeyType, ValueType, IteratorType>::operator++()
        -> CollectionIterator<KeyType, ValueType, IteratorType>&
    {
        current++;
        return *this;
    }

    template <typename KeyType, typename ValueType, typename IteratorType>
    auto CollectionIterator<KeyType, ValueType, IteratorType>::operator++(int)
        -> CollectionIterator<KeyType, ValueType, IteratorType>
    {
        CollectionIterator<KeyType, ValueType, IteratorType> temp = *this;
        ++(*this);

        return temp;
    }

    template <typename KeyType, typename ValueType, typename IteratorType>
    auto CollectionIterator<KeyType, ValueType, IteratorType>::operator==(
        const CollectionIterator<KeyType, ValueType, IteratorType>& compare) const -> bool
    {
        return compare.current == current;
    }

    template <typename KeyType, typename ValueType, typename IteratorType>
    auto CollectionIterator<KeyType, ValueType, IteratorType>::operator!=(
        const CollectionIterator<KeyType, ValueType, IteratorType>& compare) const -> bool
    {
        return compare.current != current;
    }
} // namespace UKControllerPluginUtils::Collection
