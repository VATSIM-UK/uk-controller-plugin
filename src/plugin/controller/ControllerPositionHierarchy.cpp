#include "ControllerPosition.h"
#include "ControllerPositionHierarchy.h"

using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin::Controller {
    /*
        Adds a position to the hierarchy.
    */
    void ControllerPositionHierarchy::AddPosition(std::shared_ptr<ControllerPosition> position)
    {
        this->positions.push_back(position);
    }

    /*
        Returns the size of the hierarchy.
    */
    auto ControllerPositionHierarchy::CountPositions() const -> size_t
    {
        return this->positions.size();
    }

    /*
        Returns true iff a given position is found in the hierarchy.
    */
    auto ControllerPositionHierarchy::PositionInHierarchy(const ControllerPosition& position) const -> bool
    {
        return std::find_if(
                   this->positions.cbegin(),
                   this->positions.cend(),
                   [&position](const std::shared_ptr<const ControllerPosition>& storedPosition) -> bool {
                       return position == *storedPosition;
                   }) != this->positions.cend();
    }

    auto ControllerPositionHierarchy::operator==(const ControllerPositionHierarchy& compare) const -> bool
    {
        if (compare.CountPositions() != this->CountPositions()) {
            return false;
        }

        auto it1 = this->positions.cbegin();
        auto it2 = compare.cbegin();

        // Loop through the positions, stop if they don't match
        while (it1 != this->positions.cend()) {
            if ((*it1) == (*it2)) {
                it1++;
                it2++;
                continue;
            }

            return false;
        }

        return true;
    }

    /**
     * Does the position in a given hierarchy preceed another.
     *
     * If the position is not in the hierarchy, then the answer is no, it does not.
     */
    auto ControllerPositionHierarchy::PositionPreceeds(
        const ControllerPosition& position, const ControllerPosition& preceeds) const -> bool
    {
        for (const auto& hierarchyPosition : this->positions) {
            if (*hierarchyPosition == position) {
                return true;
            }

            if (*hierarchyPosition == preceeds) {
                return false;
            }
        }

        return false;
    }
} // namespace UKControllerPlugin::Controller
