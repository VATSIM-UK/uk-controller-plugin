#include "ClearCacheOnRunwayDialogSave.h"
#include "DepartureHandoffResolver.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Handoff {

    InvalidateHandoffsOnRunwayDialogSave::InvalidateHandoffsOnRunwayDialogSave(
        std::shared_ptr<DepartureHandoffResolver> resolver, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : resolver(std::move(resolver)), plugin(plugin)
    {
        assert(resolver && "Resolver not set in ctor");
    }

    /**
     * If the runway dialog changes, we have to assume all resolutions are bad, but we can re-resolve
     * them as soon as we invalidate them.
     */
    void InvalidateHandoffsOnRunwayDialogSave::RunwayDialogSaved()
    {
        plugin.ApplyFunctionToAllFlightplans([this](
                                                 const std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface>& fp,
                                                 const std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface>& rt) {
            resolver->Invalidate(*fp);
            static_cast<void>(resolver->Resolve(*fp));
        });
    }
} // namespace UKControllerPlugin::Handoff
