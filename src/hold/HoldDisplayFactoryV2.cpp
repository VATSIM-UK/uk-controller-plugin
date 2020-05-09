#include "pch/stdafx.h"
#include "hold/HoldDisplayFactoryV2.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "hold/HoldManager.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Navaids::Navaid;

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplayFactoryV2::HoldDisplayFactoryV2(
            const EuroscopePluginLoopbackInterface & plugin,
            HoldManager & holdManager,
            const NavaidCollection& navaids
        )
            : plugin(plugin), holdManager(holdManager), navaids(navaids)
        {

        }

        /*
            Create a hold display
        */
        std::unique_ptr<HoldDisplayV2> HoldDisplayFactoryV2::Create(std::string navaid) const
        {
            const Navaid& navaidData = this->navaids.GetByIdentifier(navaid);

            if (navaidData == this->navaids.invalidNavaid) {
                return nullptr;
            }

            return std::make_unique<HoldDisplayV2>(
                plugin,
                holdManager,
                navaidData
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
