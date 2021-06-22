#pragma once
#include "euroscope/EuroscopeFlightplanListInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
         * Implements a wrapper around EuroScopes flight plan list.
         */
        class EuroscopeFlightplanListWrapper : public EuroscopeFlightplanListInterface
        {
            public:
                explicit EuroscopeFlightplanListWrapper(EuroScopePlugIn::CFlightPlanList original);
                ~EuroscopeFlightplanListWrapper() override = default;
                int NumberOfColumns() override;
                void AddColumn(std::string title, int width, bool centered, std::string tagItemProvider,
                               int tagItemId, std::string leftMouseFunctionProvider,
                               int leftMouseFunctionId,
                               std::string rightMouseFunctionProvider,
                               int rightMouseFunctionId) override;
                void AddFlightplan(EuroScopeCFlightPlanInterface& flightplan) override;
                void AddFlightplan(std::shared_ptr<EuroScopeCFlightPlanInterface> flightplan) override;
                void RemoveFlightplan(EuroScopeCFlightPlanInterface& flightplan) override;
                void RemoveFlightplan(std::shared_ptr<EuroScopeCFlightPlanInterface> flightplan) override;
                void Show() override;
                void Hide() override;

            private:

                // The original data
                EuroScopePlugIn::CFlightPlanList original;
        };

    } // namespace Euroscope
} // namespace UKControllerPlugin
