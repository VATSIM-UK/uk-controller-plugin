#pragma once
#include <string>
#include <string_view>

namespace UKControllerPlugin::Stands {
    /*
        Represents a stand assignment with its source (how it was assigned).
    */
    struct StandAssignment
    {
        enum class Source
        {
            Unknown,
            User,
            ReservationAllocator,
            VaaAllocator,
            SystemAuto,
        };

        int standId;

        // The source of the assignment (user, reservation_allocator, vaa_allocator, system_auto)
        Source source;

        [[nodiscard]] static constexpr auto ToString(Source source) -> std::string_view
        {
            using enum Source;

            switch (source) {
            case Unknown:
                return "unknown";
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

            if (source == "system_auto") {
                return SystemAuto;
            }

            return Unknown;
        }
    };
} // namespace UKControllerPlugin::Stands
