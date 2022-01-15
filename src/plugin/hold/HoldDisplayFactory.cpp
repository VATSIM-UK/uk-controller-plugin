#include "AddToHoldCallsignProvider.h"
#include "HoldDisplayFactory.h"
#include "HoldManager.h"
#include "PublishedHoldCollection.h"
#include "aircraft/CallsignSelectionList.h"
#include "aircraft/CallsignSelectionListFactory.h"
#include "dialog/DialogManager.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "navaids/NavaidCollection.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Navaids::NavaidCollection;

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplayFactory::HoldDisplayFactory(
            EuroscopePluginLoopbackInterface& plugin,
            HoldManager& holdManager,
            const NavaidCollection& navaids,
            const PublishedHoldCollection& holds,
            const DialogManager& dialogManager,
            const Aircraft::CallsignSelectionListFactory& addAircraftListFactory)
            : plugin(plugin), holdManager(holdManager), navaids(navaids), holds(holds), dialogManager(dialogManager),
              addAircraftListFactory(addAircraftListFactory)
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
                holds,
                dialogManager,
                addAircraftListFactory.Create(
                    std::make_shared<AddToHoldCallsignProvider>(navaidData, holdManager, plugin),
                    "Add to hold " + navaidData.identifier));
        }
    } // namespace Hold
} // namespace UKControllerPlugin
