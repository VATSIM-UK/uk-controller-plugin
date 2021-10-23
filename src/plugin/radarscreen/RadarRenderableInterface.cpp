#include "RadarRenderableInterface.h"

namespace UKControllerPlugin::RadarScreen {

    RadarRenderableInterface::RadarRenderableInterface() = default;
    RadarRenderableInterface::RadarRenderableInterface(const RadarRenderableInterface&) = default;
    RadarRenderableInterface::RadarRenderableInterface(RadarRenderableInterface&&) noexcept = default;
    auto RadarRenderableInterface::operator=(const RadarRenderableInterface&) -> RadarRenderableInterface& = default;
    auto RadarRenderableInterface::operator=(RadarRenderableInterface&&) noexcept
        -> RadarRenderableInterface& = default;

    /**
     * Some renderers, such as the Departure Release Request List have no left click functionality.
     */
    void RadarRenderableInterface::LeftClick(
        UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
    }

    /**
     * Some renderers, such as the HoldRenderer have no right click functionality.
     */
    void RadarRenderableInterface::RightClick(
        int objectId,
        const std::string& objectDescription,
        UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
    }

    /**
     * Some renderers, such as the PrenoteStatusView are fixed position
     * for a brief period of time, so this is a useful default.
     */
    void RadarRenderableInterface::ResetPosition()
    {
    }

    /**
     * Some renderers, such as the MissedApproachRenderer and HistoryTrailRenderer aren't draggable
     * and therefore not movable.
     */
    void RadarRenderableInterface::Move(RECT position, std::string objectDescription)
    {
    }
} // namespace UKControllerPlugin::RadarScreen
