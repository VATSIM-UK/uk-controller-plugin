#pragma once

namespace UKControllerPlugin::Prenote {
    struct PublishedPrenote;

    /**
     * Stores all the published prenotes
     */
    class PublishedPrenoteCollection
    {
        public:
        void Add(const std::shared_ptr<PublishedPrenote>& prenote);
        [[nodiscard]] auto Get(int id) const -> std::shared_ptr<PublishedPrenote>;
        [[nodiscard]] auto Count() const -> size_t;

        private:
        std::map<int, std::shared_ptr<PublishedPrenote>> prenotes;
    };
} // namespace UKControllerPlugin::Prenote
