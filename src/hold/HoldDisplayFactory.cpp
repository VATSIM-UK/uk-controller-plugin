#include "pch/stdafx.h"
#include "hold/HoldDisplayFactory.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "hold/HoldManager.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplayFactory::HoldDisplayFactory(
            const EuroscopePluginLoopbackInterface & plugin,
            HoldManager & holdManager
        )
            : plugin(plugin), holdManager(holdManager)
        {

        }

        /*
            Create a hold display
        */
        std::unique_ptr<HoldDisplay> HoldDisplayFactory::Create(unsigned int holdId) const
        {
            const ManagedHold * const managed = this->holdManager.GetManagedHold(holdId);
            if (managed == nullptr) {
                return nullptr;
            }

            return std::make_unique<HoldDisplay>(
                plugin,
                *managed,
                holdManager
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
