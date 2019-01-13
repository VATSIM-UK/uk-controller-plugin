#pragma once
// Forward declarations
namespace UKControllerPlugin {
    namespace Tag {
        class TagItemInterface;
    }  // namespace Tag
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Tag {

        /*
            A collection of TAG items, which can be called to
            produce data for a TAG.
        */
        class TagItemCollection
        {
            public:
                int CountHandlers(void) const;
                bool HasHandlerForItemId(int id) const;
                void RegisterTagItem(int itemId, std::shared_ptr<UKControllerPlugin::Tag::TagItemInterface> tagItem);
                void TagItemUpdate(
                    int tagItemId,
                    char itemData[16],
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) const;
                void RegisterAllItemsWithEuroscope(
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & pluginCore
                ) const;

                // The tag item string to use if the tag item is too long.
                const std::string invalidTagItemText = "INVALID";

            private:
                // All registered tag items
                std::map<int, std::shared_ptr<UKControllerPlugin::Tag::TagItemInterface>> tagItems;
        };
    }  // namespace Tag
}  // namespace UKControllerPlugin
