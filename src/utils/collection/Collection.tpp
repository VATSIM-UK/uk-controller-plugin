namespace UKControllerPluginUtils::Collection {
    template <typename KeyType, typename ValueType>
    void Collection<KeyType, ValueType>::Add(std::shared_ptr<ValueType> item)
    {
        const auto lock = this->GetLock();

        if (this->items.contains(item->CollectionKey())) {
            return;
        }

        this->items[item->CollectionKey()] = item;
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::FirstOrDefault(
        const std::function<bool(const std::shared_ptr<ValueType>&)>& predicate) const
        -> const std::shared_ptr<ValueType>
    {
        const auto lock = this->GetLock();
        for (const auto& item : this->items) {
            if (predicate(item.second)) {
                return item.second;
            }
        }

        return nullptr;
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::Get(const KeyType& key) const -> std::shared_ptr<ValueType>
    {
        const auto lock = this->GetLock();
        if (!this->items.contains(key)) {
            return nullptr;
        }

        return this->items.at(key);
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::Get(const std::shared_ptr<ValueType>& item) const -> std::shared_ptr<ValueType>
    {
        // No need to lock here, as Get() is already thread-safe.
        return this->Get(item->CollectionKey());
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::Contains(const KeyType& key) const -> bool
    {
        const auto lock = this->GetLock();
        return this->items.contains(key);
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::Contains(const std::shared_ptr<ValueType>& item) const -> bool
    {
        // No need to lock here, as Contains() is already thread-safe.
        return this->Contains(item->CollectionKey());
    }

    template <typename KeyType, typename ValueType> auto Collection<KeyType, ValueType>::Count() const -> size_t
    {
        const auto lock = this->GetLock();
        return this->items.size();
    }

    template <typename KeyType, typename ValueType> void Collection<KeyType, ValueType>::RemoveByKey(const KeyType& key)
    {
        const auto lock = this->GetLock();
        this->items.erase(key);
    }

    template <typename KeyType, typename ValueType>
    void Collection<KeyType, ValueType>::Remove(const std::shared_ptr<ValueType>& item)
    {
        if (item == nullptr) {
            return;
        }

        // No need to lock here, as RemoveByKey() is already thread-safe.
        this->RemoveByKey(item->CollectionKey());
    }

    template <typename KeyType, typename ValueType>
    void
    Collection<KeyType, ValueType>::RemoveWhere(const std::function<bool(const std::shared_ptr<ValueType>&)>& predicate)
    {
        const auto lock = this->GetLock();
        for (auto it = this->items.begin(); it != this->items.end();) {
            if (predicate(it->second)) {
                it = this->items.erase(it);
            } else {
                ++it;
            }
        }
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::begin()
        -> CollectionIterator<KeyType, ValueType, typename std::map<KeyType, std::shared_ptr<ValueType>>::iterator>
    {
        const auto lock = this->GetLock();
        return CollectionIterator<KeyType, ValueType, typename std::map<KeyType, std::shared_ptr<ValueType>>::iterator>(
            this->mutex, items.begin());
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::end()
        -> CollectionIterator<KeyType, ValueType, typename std::map<KeyType, std::shared_ptr<ValueType>>::iterator>
    {
        const auto lock = this->GetLock();
        return CollectionIterator<KeyType, ValueType, typename std::map<KeyType, std::shared_ptr<ValueType>>::iterator>(
            this->mutex, items.end());
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::cbegin() const -> CollectionIterator<
        KeyType,
        ValueType,
        typename std::map<KeyType, std::shared_ptr<ValueType>>::const_iterator>
    {
        const auto lock = this->GetLock();
        return CollectionIterator<
            KeyType,
            ValueType,
            typename std::map<KeyType, std::shared_ptr<ValueType>>::const_iterator>(this->mutex, items.cbegin());
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::cend() const -> CollectionIterator<
        KeyType,
        ValueType,
        typename std::map<KeyType, std::shared_ptr<ValueType>>::const_iterator>
    {
        const auto lock = this->GetLock();
        return CollectionIterator<
            KeyType,
            ValueType,
            typename std::map<KeyType, std::shared_ptr<ValueType>>::const_iterator>(this->mutex, items.cend());
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::rbegin() -> CollectionIterator<
        KeyType,
        ValueType,
        typename std::map<KeyType, std::shared_ptr<ValueType>>::reverse_iterator>
    {
        const auto lock = this->GetLock();
        return CollectionIterator<
            KeyType,
            ValueType,
            typename std::map<KeyType, std::shared_ptr<ValueType>>::reverse_iterator>(this->mutex, items.rbegin());
    }

    template <typename KeyType, typename ValueType>
    auto Collection<KeyType, ValueType>::rend() -> CollectionIterator<
        KeyType,
        ValueType,
        typename std::map<KeyType, std::shared_ptr<ValueType>>::reverse_iterator>
    {
        const auto lock = this->GetLock();
        return CollectionIterator<
            KeyType,
            ValueType,
            typename std::map<KeyType, std::shared_ptr<ValueType>>::reverse_iterator>(this->mutex, items.rend());
    }
} // namespace UKControllerPluginUtils::Collection
