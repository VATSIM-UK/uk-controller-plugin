#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"

namespace UKControllerPlugin::Prenote {
    void PrenoteMessageCollection::Add(const std::shared_ptr<PrenoteMessage>& prenote)
    {
        auto lock = this->LockPrenotes();
        if (!this->prenotes.insert(prenote).second) {
            LogWarning("Tried to add duplicate prenote");
        }
    }

    void PrenoteMessageCollection::Remove(int id)
    {
        auto lock = this->LockPrenotes();
        auto prenote = this->GetByIdUnsafe(id);
        if (prenote == nullptr) {
            LogWarning("Tried to delete non-existent prenote");
            return;
        }

        this->prenotes.erase(prenote);
    }

    auto PrenoteMessageCollection::Count() const -> size_t
    {
        return this->prenotes.size();
    }

    auto PrenoteMessageCollection::GetById(int id) -> const std::shared_ptr<PrenoteMessage>&
    {
        auto lock = this->LockPrenotes();
        return this->GetByIdUnsafe(id);
    }

    auto PrenoteMessageCollection::LockPrenotes() -> std::lock_guard<std::mutex>
    {
        return std::lock_guard(this->prenoteLock);
    }

    auto PrenoteMessageCollection::GetByIdUnsafe(int id) -> const std::shared_ptr<PrenoteMessage>&
    {
        auto prenote = this->prenotes.find(id);
        return prenote == this->prenotes.cend() ? this->invalidMessage : *prenote;
    }

    void PrenoteMessageCollection::Iterate(const std::function<void(const std::shared_ptr<PrenoteMessage>&)>& function)
    {
        std::lock_guard<std::mutex> lock = this->LockPrenotes();
        for (const auto& message : this->prenotes) {
            function(message);
        }
    }
} // namespace UKControllerPlugin::Prenote
