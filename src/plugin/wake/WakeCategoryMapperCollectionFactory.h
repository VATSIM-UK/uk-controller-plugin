#pragma once

namespace UKControllerPlugin::Aircraft {
    class AircraftTypeMapperInterface;
} // namespace UKControllerPlugin::Aircraft

namespace UKControllerPlugin::Wake {
    class WakeCategoryMapperCollection;
    class WakeSchemeCollection;

    [[nodiscard]] auto MakeMapperCollection(
        const WakeSchemeCollection& collection, const Aircraft::AircraftTypeMapperInterface& aircraftTypeMapper)
        -> std::unique_ptr<WakeCategoryMapperCollection>;
} // namespace UKControllerPlugin::Wake
