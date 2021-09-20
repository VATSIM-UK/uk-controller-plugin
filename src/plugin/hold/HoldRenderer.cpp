#include "HoldDisplayFactory.h"
#include "HoldManager.h"
#include "HoldRenderer.h"
#include "euroscope/UserSetting.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::GdiGraphicsInterface;

namespace UKControllerPlugin::Hold {

    HoldRenderer::HoldRenderer(
        std::shared_ptr<const HoldDisplayManager> displays,
        const int screenObjectId,
        const int toggleCallbackFunctionId)
        : displays(std::move(displays)), screenObjectId(screenObjectId),
          toggleCallbackFunctionId(toggleCallbackFunctionId)
    {
    }

    /*
        Load user settings from the ASR - whether to render the hold displays.
    */
    void HoldRenderer::AsrLoadedEvent(UserSetting& userSetting)
    {
        this->renderHolds = userSetting.GetBooleanEntry(this->asrVisibleKey, true);
    }

    /*
        Save user settings to the ASR - whether to render the hold displays.
    */
    void HoldRenderer::AsrClosingEvent(UserSetting& userSetting)
    {
        userSetting.Save(this->asrVisibleKey, this->asrVisibleDescription, this->renderHolds);
    }

    /*
        Should holds be rendered
    */
    auto HoldRenderer::IsVisible() const -> bool
    {
        return this->renderHolds;
    }

    /*
        A button has been left clicked
    */
    void HoldRenderer::LeftClick(
        EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        std::string holdName = GetHoldNameFromObjectDescription(objectDescription);
        auto display = std::find_if(
            this->displays->cbegin(),
            this->displays->cend(),
            [&holdName](const std::unique_ptr<HoldDisplay>& hold) -> bool {
                return hold->navaid.identifier == holdName;
            });

        if (display == this->displays->cend()) {
            LogWarning("Tried to interact with invalid hold display");
            return;
        }

        if (objectDescription.find("cleared") != std::string::npos) {
            (*display)->ClearedLevelClicked(
                GetCallsignFromObjectDescription(objectDescription), radarScreen, mousePos, itemArea);
            return;
        }

        if (objectDescription.find("callsign") != std::string::npos) {
            (*display)->CallsignClicked(
                GetCallsignFromObjectDescription(objectDescription), radarScreen, mousePos, itemArea);
            return;
        }

        (*display)->ButtonClicked(GetButtonNameFromObjectDescription(objectDescription));
    }

    /*
        Move a given hold display
    */
    void HoldRenderer::Move(RECT position, std::string objectDescription)
    {
        std::string holdName = GetHoldNameFromObjectDescription(objectDescription);
        auto display = std::find_if(
            this->displays->cbegin(),
            this->displays->cend(),
            [&holdName](const std::unique_ptr<HoldDisplay>& hold) -> bool {
                return hold->navaid.identifier == holdName;
            });

        if (display == this->displays->cend()) {
            LogWarning("Tried to move invalid hold display");
            return;
        }

        (*display)->Move({position.left, position.top});
    }

    /*
        Render all the hold displays
    */
    void HoldRenderer::Render(GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        for (auto it = this->displays->cbegin(); it != this->displays->cend(); ++it) {
            (*it)->PaintWindow(graphics, radarScreen, this->screenObjectId);
        }
    }

    /*
        Set whether or not holds should be rendered.
    */
    void HoldRenderer::SetVisible(bool visible)
    {
        this->renderHolds = visible;
    }

    /*
        Reset the position of each hold display
    */
    void HoldRenderer::ResetPosition()
    {
        for (auto it = this->displays->cbegin(); it != this->displays->cend(); ++it) {
            (*it)->Move(DEFAULT_POSITION);
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
    auto HoldRenderer::GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = this->menuItemDescription;
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->toggleCallbackFunctionId;
        returnVal.checked =
            this->renderHolds ? EuroScopePlugIn::POPUP_ELEMENT_CHECKED : EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;
        return returnVal;
    }

    /*
        Given an item description, return the hold id
    */
    auto HoldRenderer::GetHoldNameFromObjectDescription(const std::string& objectDescription) -> std::string
    {
        return objectDescription.substr(0, objectDescription.find('/'));
    }

    auto HoldRenderer::GetCallsignFromObjectDescription(const std::string& objectDescription) -> std::string
    {
        return objectDescription.substr(objectDescription.find_last_of('/') + 1);
    }

    /*
        Given an item description, return the button name
    */
    auto HoldRenderer::GetButtonNameFromObjectDescription(const std::string& objectDescription) -> std::string
    {
        return objectDescription.substr(objectDescription.find('/') + 1);
    }

    auto HoldRenderer::GetToggleCallbackId() const -> int
    {
        return this->toggleCallbackFunctionId;
    }
} // namespace UKControllerPlugin::Hold
