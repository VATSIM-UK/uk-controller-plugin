#include "PublishedPrenote.h"
#include "PublishedPrenoteCollection.h"

namespace UKControllerPlugin::Prenote {
    void PublishedPrenoteCollection::Add(const std::shared_ptr<PublishedPrenote>& prenote)
    {
        if (this->prenotes.count(prenote->id) != 0) {
            LogWarning("Duplicate published prenote added");
            return;
        }

        this->prenotes[prenote->id] = prenote;
    }

    auto PublishedPrenoteCollection::Get(int id) const -> std::shared_ptr<PublishedPrenote>
    {
        auto prenote = this->prenotes.find(id);
        return prenote == this->prenotes.cend() ? nullptr : prenote->second;
    }

    auto PublishedPrenoteCollection::Count() const -> size_t
    {
        return this->prenotes.size();
    }
} // namespace UKControllerPlugin::Prenote
