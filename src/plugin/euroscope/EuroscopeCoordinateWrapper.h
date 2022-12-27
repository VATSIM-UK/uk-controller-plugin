#include "EuroscopeCoordinateInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeCoordinateWrapper: public EuroscopeCoordinateInterface
    {
        public:
            EuroscopeCoordinateWrapper(EuroScopePlugIn::CPosition original);
            [[nodiscard]] auto DirectionTo(const EuroscopeCoordinateInterface& coordinate) const -> double override;
            [[nodiscard]] auto ToEuroscopePosition() const -> const EuroScopePlugIn::CPosition& override;

        private:
            EuroScopePlugIn::CPosition original;      
    };
} // namespace UKControllerPlugin::Euroscope