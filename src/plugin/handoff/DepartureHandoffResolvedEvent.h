#pragma once

namespace UKControllerPlugin::Handoff {
    struct ResolvedHandoff;

    using DepartureHandoffResolvedEvent = struct DepartureHandoffResolvedEvent
    {
        std::shared_ptr<const ResolvedHandoff> handoff;
    };
} // namespace UKControllerPlugin::Handoff
