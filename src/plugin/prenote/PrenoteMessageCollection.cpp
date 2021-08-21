#include "PrenoteMessageCollection.h"
#include "PrenoteMessage.h"

namespace UKControllerPlugin::Prenote {
    void PrenoteMessageCollection::Add(const std::shared_ptr<PrenoteMessage>& prenote)
    {
        if (!this->prenotes.insert(prenote).second) {
            LogWarning("Tried to add duplicate prenote");
        }
    }

    void PrenoteMessageCollection::Remove(int id)
    {
        auto prenote = this->GetById(id);
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
        auto prenote = this->prenotes.find(id);
        return prenote == this->prenotes.cend() ? this->invalidMessage : *prenote;
    }

    auto PrenoteMessageCollection::cbegin() const noexcept -> PrenoteMessages::const_iterator
    {
        return this->prenotes.cbegin();
    }

    auto PrenoteMessageCollection::cend() const noexcept -> PrenoteMessages::const_iterator
    {
        return this->prenotes.cend();
    }
} // namespace UKControllerPlugin::Prenote
