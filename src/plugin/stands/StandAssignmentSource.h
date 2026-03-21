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
            switch (source) {
            case Source::User:
                return "user";
            case Source::ReservationAllocator:
                return "reservation_allocator";
            case Source::VaaAllocator:
                return "vaa_allocator";
            case Source::SystemAuto:
                return "system_auto";
            }

            return "system_auto";
        }

        [[nodiscard]] static constexpr auto FromString(std::string_view source) -> Source
        {
            if (source == "user") {
                return Source::User;
            }
            if (source == "reservation_allocator") {
                return Source::ReservationAllocator;
            }
            if (source == "vaa_allocator") {
                return Source::VaaAllocator;
            }

            return Source::SystemAuto;
        }
    };
} // namespace UKControllerPlugin::Stands
