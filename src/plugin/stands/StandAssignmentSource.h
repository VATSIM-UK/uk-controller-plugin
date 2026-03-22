#pragma once
#include <string>
#include <string_view>

namespace UKControllerPlugin::Stands {
    /*
        Represents a stand assignment with its source (how it was assigned).
    */
    struct StandAssignmentSource
    {
        enum class Source
        {
            User,
            ReservationAllocator,
            VaaAllocator,
            SystemAuto,
        };

        int standId;

        // The source of the assignment (user, reservation_allocator, vaa_allocator, system_auto)
        Source source;

        // The four possible sources for stand assignments
        static constexpr Source SOURCE_USER = Source::User;
        static constexpr Source SOURCE_RESERVATION_ALLOCATOR = Source::ReservationAllocator;
        static constexpr Source SOURCE_VAA_ALLOCATOR = Source::VaaAllocator;
        static constexpr Source SOURCE_SYSTEM = Source::SystemAuto;

        [[nodiscard]] static constexpr auto ToString(Source source) -> std::string_view
        {
            using enum Source;

            switch (source) {
            case User:
                return "user";
            case ReservationAllocator:
                return "reservation_allocator";
            case VaaAllocator:
                return "vaa_allocator";
            case SystemAuto:
                return "system_auto";
            }

            return "system_auto";
        }

        [[nodiscard]] static constexpr auto FromString(std::string_view source) -> Source
        {
            using enum Source;

            if (source == "user") {
                return User;
            }
            if (source == "reservation_allocator") {
                return ReservationAllocator;
            }
            if (source == "vaa_allocator") {
                return VaaAllocator;
            }

            return SystemAuto;
        }
    };
} // namespace UKControllerPlugin::Stands
