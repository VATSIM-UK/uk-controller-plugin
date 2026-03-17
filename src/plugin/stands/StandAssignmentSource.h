#pragma once
#include <string>
#include <string_view>

namespace UKControllerPlugin::Stands {
    /*
        Represents a stand assignment with its source (how it was assigned).
    */
    struct StandAssignmentSource
    {
        int standId;

        // The source of the assignment (user, reservation_allocator, vaa_allocator, system_auto)
        std::string source;

        // The four possible sources for stand assignments
        static constexpr std::string_view SOURCE_USER = "user";
        static constexpr std::string_view SOURCE_RESERVATION_ALLOCATOR = "reservation_allocator";
        static constexpr std::string_view SOURCE_VAA_ALLOCATOR = "vaa_allocator";
        static constexpr std::string_view SOURCE_SYSTEM = "system_auto";
    };
} // namespace UKControllerPlugin::Stands
