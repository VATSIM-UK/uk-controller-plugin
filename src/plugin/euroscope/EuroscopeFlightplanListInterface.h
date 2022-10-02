#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {

        class EuroScopeCFlightPlanInterface;

        /*
         * Represents a list of flightplans in EuroScope.
         */
        class EuroscopeFlightplanListInterface
        {
            public:
            virtual ~EuroscopeFlightplanListInterface() = default;

            virtual int NumberOfColumns() = 0;
            virtual void AddColumn(
                std::string title,
                int width,
                bool centered,
                std::string tagItemProvider,
                int tagItemId,
                std::string leftMouseFunctionProvider,
                int leftMouseFunctionId,
                std::string rightMouseFunctionProvider,
                int rightMouseFunctionId) = 0;
            virtual void AddFlightplan(EuroScopeCFlightPlanInterface& flightplan) = 0;
            virtual void AddFlightplan(std::shared_ptr<EuroScopeCFlightPlanInterface> flightplan) = 0;
            virtual void RemoveFlightplan(EuroScopeCFlightPlanInterface& flightplan) = 0;
            virtual void RemoveFlightplan(std::shared_ptr<EuroScopeCFlightPlanInterface> flightplan) = 0;
            virtual void Show() = 0;
            virtual void Hide() = 0;

            inline static const std::string EUROSCOPE_PROVIDER = "";
            inline static const std::string UKCP_PROVIDER = "UK Controller Plugin";
        };

    } // namespace Euroscope
} // namespace UKControllerPlugin
