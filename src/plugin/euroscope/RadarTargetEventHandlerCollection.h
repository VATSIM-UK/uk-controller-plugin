#pragma once
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCRadarTargetInterface;
        class RadarTargetEventHandlerInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            A repository of event handlers for RadarTarget events. When an event is received, it will
            call each of the handlers in turn.
        */
        class RadarTargetEventHandlerCollection
        {
            public:
            int CountHandlers(void) const;
            void RadarTargetEvent(UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) const;
            void RegisterHandler(std::shared_ptr<RadarTargetEventHandlerInterface> handler);

            private:
            // Set of registered handlers
            std::set<std::shared_ptr<RadarTargetEventHandlerInterface>> handlerList;
        };
    } // namespace Euroscope
} // namespace UKControllerPlugin
