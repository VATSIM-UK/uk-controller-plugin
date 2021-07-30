#include "pch/stdafx.h"
#include "stands/CompareStands.h"

namespace UKControllerPlugin {
    namespace Stands {

        bool CompareStands::operator()(const Stand& stand, int id) const
        {
            return stand.id < id;
        }

        bool CompareStands::operator()(int id, const Stand& stand) const
        {
            return id < stand.id;
        }

        bool CompareStands::operator()(
            const Stand& standA,
            const Stand& standB
            ) const {
            return standA.id < standB.id;
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
