#include "MockEuroscopePluginLoopbackInterface.h"

namespace UKControllerPluginTest::Euroscope {
    MockEuroscopePluginLoopbackInterface::MockEuroscopePluginLoopbackInterface() = default;
    MockEuroscopePluginLoopbackInterface::~MockEuroscopePluginLoopbackInterface() = default;

    void MockEuroscopePluginLoopbackInterface::AddAllFlightplansItem(MockFlightplanRadarTargetPair item)
    {
        this->allFpRtPairs.push_back(item);
    }

    void MockEuroscopePluginLoopbackInterface::AddAllControllersItem(
        std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCControllerInterface> item)
    {
        this->allControllers.push_back(item);
    }

    void MockEuroscopePluginLoopbackInterface::ApplyFunctionToAllFlightplans(
        std::function<void(
            std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>,
            std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>)> function)
    {
        if (this->expectFlightplanLoopNoFire) {
            throw std::logic_error("Was not expecting the flightplan loop");
        }

        for (auto it = this->allFpRtPairs.cbegin(); it != this->allFpRtPairs.cend(); ++it) {
            function(it->fp, it->rt);
        }
    };

    void MockEuroscopePluginLoopbackInterface::ApplyFunctionToAllFlightplans(
        std::function<void(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&)> function)
    {
        if (this->expectFlightplanLoopNoFire) {
            throw std::logic_error("Was not expecting the flightplan loop");
        }

        for (auto it = this->allFpRtPairs.cbegin(); it != this->allFpRtPairs.cend(); ++it) {
            function(*it->fp, *it->rt);
        }
    };

    void MockEuroscopePluginLoopbackInterface::ApplyFunctionToAllFlightplans(
        std::function<void(
            const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
            const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&)> function) const
    {
        if (this->expectFlightplanLoopNoFire) {
            throw std::logic_error("Was not expecting the flightplan loop");
        }

        for (auto it = this->allFpRtPairs.cbegin(); it != this->allFpRtPairs.cend(); ++it) {
            function(*it->fp, *it->rt);
        }
    };

    void MockEuroscopePluginLoopbackInterface::ApplyFunctionToAllControllers(
        std::function<void(std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCControllerInterface>)> function)
    {
        for (auto it = this->allControllers.cbegin(); it != this->allControllers.cend(); ++it) {
            function(*it);
        }
    }

    void MockEuroscopePluginLoopbackInterface::SetEuroscopeSelectedFlightplan(
        std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface> flightplan)
    {
        this->SetEuroscopeSelectedFlightplanPointer(flightplan);
    }

    void MockEuroscopePluginLoopbackInterface::SetEuroscopeSelectedFlightplan(
        const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan)
    {
        this->SetEuroscopeSelectedFlightplanReference(flightplan);
    }

    void MockEuroscopePluginLoopbackInterface::ExpectNoFlightplanLoop()
    {
        this->expectFlightplanLoopNoFire = true;
    }
} // namespace UKControllerPluginTest::Euroscope
