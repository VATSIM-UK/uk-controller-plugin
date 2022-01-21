#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPosition;

    /*
        Represents a hierarchy of controllers.
    */
    class ControllerPositionHierarchy
    {
        public:
        ControllerPositionHierarchy() = default;
        void AddPosition(std::shared_ptr<ControllerPosition> position);
        [[nodiscard]] auto CountPositions() const -> size_t;
        [[nodiscard]] auto PositionInHierarchy(const UKControllerPlugin::Controller::ControllerPosition& position) const
            -> bool;
        [[nodiscard]] auto PositionPreceeds(
            const UKControllerPlugin::Controller::ControllerPosition& position,
            const UKControllerPlugin::Controller::ControllerPosition& preceeds) const -> bool;
        [[nodiscard]] auto operator==(const UKControllerPlugin::Controller::ControllerPositionHierarchy& compare) const
            -> bool;

        // Public type definitions for a custom iterator over the class.
        using PositionHierarchy = std::list<std::shared_ptr<ControllerPosition>>;
        using const_iterator = PositionHierarchy::const_iterator;
        [[nodiscard]] const_iterator begin() const
        {
            return positions.begin();
        }
        [[nodiscard]] const_iterator cbegin() const
        {
            return positions.cbegin();
        }
        [[nodiscard]] const_iterator end() const
        {
            return positions.end();
        }
        [[nodiscard]] const_iterator cend() const
        {
            return positions.cend();
        }

        private:
        PositionHierarchy positions;
    };
} // namespace UKControllerPlugin::Controller
