#include "pch/stdafx.h"
#include "hold/HoldRenderer.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFactory.h"
#include "helper/HelperFunctions.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Hold {

        HoldRenderer::HoldRenderer(
            const std::shared_ptr<const HoldDisplayManager> displays,
            const int screenObjectId,
            const int toggleCallbackFunctionId
        )
            : displays(displays), screenObjectId(screenObjectId), toggleCallbackFunctionId(toggleCallbackFunctionId)
        {

        }

        /*
            Should holds be rendered
        */
        bool HoldRenderer::IsVisible(void) const
        {
            return this->renderHolds;
        }

        /*
            A button has been left clicked
        */
        void HoldRenderer::LeftClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            int holdId = this->GetHoldIdFromObjectDescription(objectDescription);
            auto display = std::find_if(
                this->displays->cbegin(),
                this->displays->cend(),
                [holdId](const std::unique_ptr<HoldDisplay> & hold) -> bool {
                    return hold->managedHold.GetHoldParameters().identifier == holdId;
                }
            );

            if (display == this->displays->cend()) {
                LogWarning("Tried to move invalid hold display");
                return;
            }

            (*display)->ButtonClicked(this->GetButtonNameFromObjectDescription(objectDescription));
        }

        /*
            For now, nothing accepts a right click
        */
        void HoldRenderer::RightClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {

        }

        /*
            Move a given hold display
        */
        void HoldRenderer::Move(RECT position, std::string objectDescription)
        {
            int holdId = this->GetHoldIdFromObjectDescription(objectDescription);
            auto display = std::find_if(
                this->displays->cbegin(),
                this->displays->cend(),
                [holdId](const std::unique_ptr<HoldDisplay> & hold) -> bool {
                    return hold->managedHold.GetHoldParameters().identifier == holdId;
                }
            );

            if (display == this->displays->cend()) {
                LogWarning("Tried to move invalid hold display");
                return;
            }

            (*display)->Move({ position.left, position.top });
        }

        /*
            Render all the hold displays
        */
        void HoldRenderer::Render(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            for (
                HoldDisplayManager::const_iterator it = this->displays->cbegin();
                it != this->displays->cend();
                ++it
            ) {
                (*it)->PaintWindow(graphics, radarScreen, this->screenObjectId);
            }
        }

        /*
            Toggle hold visibility
        */
        void HoldRenderer::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->renderHolds = !this->renderHolds;
        }

        /*
            Return the configuration menu item for this class
        */
        UKControllerPlugin::Plugin::PopupMenuItem HoldRenderer::GetConfigurationMenuItem(void) const
        {
            PopupMenuItem returnVal;
            returnVal.firstValue = this->menuItemDescription;
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->toggleCallbackFunctionId;
            returnVal.checked = this->renderHolds;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        /*
            Given an item description, return the hold id
        */
        int HoldRenderer::GetHoldIdFromObjectDescription(std::string objectDescription) const
        {
            return std::stoi(objectDescription.substr(0, objectDescription.find("/")));
        }

        /*
            Given an item description, return the button name
        */
        std::string HoldRenderer::GetButtonNameFromObjectDescription(std::string objectDescription) const
        {
            return objectDescription.substr(objectDescription.find("/") + 1);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
