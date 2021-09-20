#pragma once

namespace UKControllerPlugin::Hold {
    /*
        Represents a restriction on the levels in a hold that may be utilised.
    */
    class AbstractHoldLevelRestriction
    {
        public:
        AbstractHoldLevelRestriction() = default;
        virtual ~AbstractHoldLevelRestriction() = default;
        AbstractHoldLevelRestriction(const AbstractHoldLevelRestriction&) = default;
        AbstractHoldLevelRestriction(AbstractHoldLevelRestriction&&) noexcept = default;
        auto operator=(const AbstractHoldLevelRestriction&) -> AbstractHoldLevelRestriction& = default;
        auto operator=(AbstractHoldLevelRestriction&&) noexcept -> AbstractHoldLevelRestriction& = default;

        /*
            Given a level, return true if it is off limits, false if not.
        */
        [[nodiscard]] virtual auto LevelRestricted(unsigned int level) const -> bool = 0;
    };
} // namespace UKControllerPlugin::Hold
