#pragma once
#include "push/PushEventProcessorInterface.h"
#include "tag/TagItemInterface.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Hold {
        class HoldManager;
    } // namespace Hold
    namespace Navaids {
        class NavaidCollection;
    } // namespace Navaids
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Hold {

    /*
        Handles events that relate to holding - namely the periodic
        update of holding data.
    */
    class HoldEventHandler : public UKControllerPlugin::Tag::TagItemInterface,
                             public UKControllerPlugin::TimedEvent::AbstractTimedEvent,
                             public Push::PushEventProcessorInterface
    {
        public:
        HoldEventHandler(
            UKControllerPlugin::Hold::HoldManager& holdManager,
            const UKControllerPlugin::Navaids::NavaidCollection& navaids,
            UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin);

        // Inherited via TagItemInterface
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

        // Inherited via AbstractTimedEvent
        void TimedEventTrigger() override;

        // Inherited via WebsocketEventProcessorInterface
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;

        private:
        // Navaids against which holds are based
        const UKControllerPlugin::Navaids::NavaidCollection& navaids;

        // Gives access to the plugin
        UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Manages holds
        UKControllerPlugin::Hold::HoldManager& holdManager;

        // The string to display when an aircraft is not holding
        const std::string noHold = "NOHOLD";

        // How far from a navaid an aircraft can be before its considered in proximity of the hold
        const double proximityDistance = 12.0;
        
        // How far from a navaid an aircraft can be before its considered to have entered the hold
        const double enterDistance = 3.0;
    };
} // namespace UKControllerPlugin::Hold
