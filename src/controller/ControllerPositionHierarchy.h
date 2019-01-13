#pragma once

namespace UKControllerPlugin {
    namespace Controller {
        class ControllerPosition;
    }  // namespace Controller
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Controller {

/*
    Represents a hierarchy of controllers.
*/
class ControllerPositionHierarchy
{
    public:
        ControllerPositionHierarchy(void);
        void AddPosition(const UKControllerPlugin::Controller::ControllerPosition & position);
        size_t CountPositions(void) const;
        bool PositionInHierarchy(const UKControllerPlugin::Controller::ControllerPosition & position) const;

        // Public type definitions for a custom iterator over the class.
        typedef std::vector<std::reference_wrapper<const ControllerPosition>> PositionHierarchy;
        typedef PositionHierarchy::const_iterator const_iterator;
        const_iterator cbegin(void) const { return positions.cbegin(); }
        const_iterator cend(void) const { return positions.cend(); }  // namespace Controller

    private:
        PositionHierarchy positions;
};

}  // namespace Controller
}  // namespace UKControllerPlugin
