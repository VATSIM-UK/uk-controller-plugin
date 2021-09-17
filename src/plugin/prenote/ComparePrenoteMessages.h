#pragma once

namespace UKControllerPlugin::Prenote {
    class PrenoteMessage;

    /**
     * Compares prenotes in a set by id.
     */
    using ComparePrenoteMessages = struct ComparePrenoteMessages
    {
        using is_transparent = int;

        auto operator()(const std::shared_ptr<PrenoteMessage>& hold, int id) const -> bool;
        auto operator()(int id, const std::shared_ptr<PrenoteMessage>& hold) const -> bool;
        auto operator()(const std::shared_ptr<PrenoteMessage>& a, const std::shared_ptr<PrenoteMessage>& b) const
            -> bool;
    };
} // namespace UKControllerPlugin::Prenote
