#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Tag {

        /*
            Represents a function that EuroScope will call through its
            OnFunctionCall interface.
        */
        typedef struct TagFunction
        {

            TagFunction();
            TagFunction(
                int functionId,
                std::string description,
                std::function<void(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&,
                    std::string context,
                    const POINT& mousePos)> function);
            TagFunction(const TagFunction& copy);
            void operator=(const UKControllerPlugin::Tag::TagFunction& copy);
            bool operator==(const TagFunction& compare) const;

            int functionId;
            std::string description;
            std::function<void(
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&,
                std::string context,
                const POINT& mousePos)>
                function;
        } TagFunction;
    } // namespace Tag
} // namespace UKControllerPlugin
