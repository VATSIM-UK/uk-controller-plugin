#include "pch/pch.h"
#include "euroscope/EuroscopeFlightplanListWrapper.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        EuroscopeFlightplanListWrapper::EuroscopeFlightplanListWrapper(
            EuroScopePlugIn::CFlightPlanList original): original(original) {}

        int EuroscopeFlightplanListWrapper::NumberOfColumns()
        {
            return this->original.GetColumnNumber();
        }

        void EuroscopeFlightplanListWrapper::AddColumn(
            std::string title,
            int width,
            bool centered,
            std::string tagItemProvider,
            int tagItemId,
            std::string leftMouseFunctionProvider,
            int leftMouseFunctionId,
            std::string rightMouseFunctionProvider,
            int rightMouseFunctionId
        )
        {
            this->original.AddColumnDefinition(
                title.c_str(),
                width,
                centered,
                tagItemProvider.c_str(),
                tagItemId,
                leftMouseFunctionProvider.c_str(),
                leftMouseFunctionId,
                rightMouseFunctionProvider.c_str(),
                rightMouseFunctionId
            );
        }

        void EuroscopeFlightplanListWrapper::AddFlightplan(
            EuroScopeCFlightPlanInterface& flightplan
        )
        {
            this->original.AddFpToTheList(flightplan.GetEuroScopeObject());
        }

        void EuroscopeFlightplanListWrapper::AddFlightplan(
            std::shared_ptr<EuroScopeCFlightPlanInterface> flightplan
        )
        {
            this->AddFlightplan(*flightplan);
        }

        void EuroscopeFlightplanListWrapper::RemoveFlightplan(EuroScopeCFlightPlanInterface& flightplan)
        {
            this->original.RemoveFpFromTheList(flightplan.GetEuroScopeObject());
        }

        void EuroscopeFlightplanListWrapper::RemoveFlightplan(
            std::shared_ptr<EuroScopeCFlightPlanInterface> flightplan
        )
        {
            this->RemoveFlightplan(*flightplan);
        }

        void EuroscopeFlightplanListWrapper::Show()
        {
            this->original.ShowFpList(true);
        }

        void EuroscopeFlightplanListWrapper::Hide()
        {
            this->original.ShowFpList(false);
        }
    } // namespace Euroscope
} // namespace UKControllerPlugin
