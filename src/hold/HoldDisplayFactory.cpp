#include "pch/stdafx.h"
#include "hold/HoldDisplayFactory.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "hold/HoldManager.h"
#include "dialog/DialogManager.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Dialog::DialogManager;

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplayFactory::HoldDisplayFactory(
            EuroscopePluginLoopbackInterface & plugin,
            HoldManager & holdManager,
            const NavaidCollection& navaids,
            const PublishedHoldCollection& holds,
            const DialogManager& dialogManager
        )
            : plugin(plugin), holdManager(holdManager), navaids(navaids),
            holds(holds), dialogManager(dialogManager)
        {

        }

        /*
            Create a hold display
        */
        std::unique_ptr<HoldDisplay> HoldDisplayFactory::Create(std::string navaid) const
        {
            const Navaid& navaidData = this->navaids.GetByIdentifier(navaid);

            if (navaidData == this->navaids.invalidNavaid) {
                return nullptr;
            }

            return std::make_unique<HoldDisplay>(
                plugin,
                holdManager,
                navaidData,
                holds.Get(navaid),
                dialogManager
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
