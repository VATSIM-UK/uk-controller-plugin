#pragma once
#include "plugin/PopupMenuItem.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroScopeCControllerInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
namespace Euroscope {

/*
    An interface implemented by classes that are registered as a plugin
    with Euroscope. This provides methods for interacting with an instance
    which in turn will interact directly with Euroscope.
*/
class EuroscopePluginLoopbackInterface
{
    public:
        virtual void AddItemToPopupList(const UKControllerPlugin::Plugin::PopupMenuItem item) = 0;
        virtual void ChatAreaMessage(
            std::string handler,
            std::string sender,
            std::string message,
            bool showHandler,
            bool markUnread,
            bool overrideBusy,
            bool flash,
            bool confirm
        ) = 0;
        virtual std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCControllerInterface> GetUserControllerObject(
            void
        ) const = 0;
        virtual int GetEuroscopeConnectionStatus(void) const = 0;
        virtual double GetDistanceFromUserVisibilityCentre(EuroScopePlugIn::CPosition position) const = 0;
        virtual std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface> GetFlightplanForCallsign(
            std::string callsign
        ) const = 0;
        virtual std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>
            GetRadarTargetForCallsign(std::string callsign) const = 0;
        virtual std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>
            GetSelectedFlightplan() const = 0;
        virtual std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>
            GetSelectedRadarTarget() const = 0;
        virtual void TriggerPopupList(RECT area, std::string title, int numColumns) = 0;
        virtual void TriggerFlightplanUpdateForCallsign(std::string callsign) = 0;
        virtual void RegisterTagFunction(int itemCode, std::string description) = 0;
        virtual void RegisterTagItem(int itemCode, std::string description) = 0;
        virtual void ApplyFunctionToAllFlightplans(
            std::function<
                void(
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>
                )
            > function
        ) = 0;
        virtual void ShowTextEditPopup(RECT editArea, int callbackId, std::string initialValue) = 0;
};
}  // namespace Euroscope
}  // namespace UKControllerPlugin
