#include "pch/stdafx.h"
#include "hold/HoldRenderer.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplay.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Hold {

        HoldRenderer::HoldRenderer(
            const HoldProfileManager & profileManager,
            const HoldManager & holdManager,
            const EuroscopePluginLoopbackInterface & plugin,
            const int screenObjectId,
            const int toggleCallbackFunctionId
        )
            : profileManager(profileManager), holdManager(holdManager), plugin(plugin), screenObjectId(screenObjectId),
            toggleCallbackFunctionId(toggleCallbackFunctionId)
        {

        }

        void HoldRenderer::AsrLoadedEvent(UserSetting & userSetting)
        {

        }

        void HoldRenderer::AsrClosingEvent(UserSetting & userSetting)
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
                this->holds.begin(),
                this->holds.end(),
                [holdId](const HoldDisplay & hold) -> bool {
                return hold.managedHold.GetHoldParameters().identifier == holdId;
            }
            );

            if (display == this->holds.cend()) {
                LogWarning("Tried to move invalid hold display");
                return;
            }

            display->ButtonClicked(this->GetButtonNameFromObjectDescription(objectDescription));
        }

        /*
            Load the given hold profile
        */
        void HoldRenderer::LoadProfile(unsigned int profileId)
        {
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
                this->holds.begin(),
                this->holds.end(),
                [holdId](const HoldDisplay & hold) -> bool {
                    return hold.managedHold.GetHoldParameters().identifier == holdId;
                }
            );

            if (display == this->holds.cend()) {
                LogWarning("Tried to move invalid hold display");
                return;
            }

            display->Move({ position.left, position.top });
        }

        /*
            Render all the hold displays
        */
        void HoldRenderer::Render(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            for (
                std::list<HoldDisplay>::const_iterator it = this->holds.cbegin();
                it != this->holds.cend();
                ++it
            ) {
                it->PaintWindow(graphics, radarScreen, this->screenObjectId);
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
