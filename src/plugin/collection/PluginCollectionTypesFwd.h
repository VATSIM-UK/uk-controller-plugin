#pragma once

namespace UKControllerPluginUtils::Collection {
    template <typename KeyType, typename ValueType> class Collection;
} // namespace UKControllerPluginUtils::Collection

namespace UKControllerPlugin::Releases {
    class DepartureReleaseRequest;

    using DepartureReleaseRequestCollection =
        UKControllerPluginUtils::Collection::Collection<int, DepartureReleaseRequest>;
} // namespace UKControllerPlugin::Releases
