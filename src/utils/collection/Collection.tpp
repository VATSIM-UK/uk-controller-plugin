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
} // namespace UKControllerPluginUtils::Collection
