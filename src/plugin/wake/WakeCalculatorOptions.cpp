#include "WakeCalculatorOptions.h"

namespace UKControllerPlugin::Wake {
    auto WakeCalculatorOptions::LeadAircraft() const -> const std::string&
    {
        return leadAircraft;
    }

    void WakeCalculatorOptions::LeadAircraft(const std::string& leadAircraft)
    {
        this->leadAircraft = leadAircraft;
        followingAircraft = "";
    }

    auto WakeCalculatorOptions::FollowingAircraft() const -> const std::string&
    {
        return followingAircraft;
    }

    void WakeCalculatorOptions::FollowingAircraft(const std::string& followingAircraft)
    {
        this->followingAircraft = followingAircraft;
    }

    auto WakeCalculatorOptions::Scheme() const -> const std::string&
    {
        return scheme;
    }

    void WakeCalculatorOptions::Scheme(const std::string& scheme)
    {
        this->scheme = scheme;
    }

    auto WakeCalculatorOptions::Intermediate() const -> bool
    {
        return intermediate;
    }

    void WakeCalculatorOptions::Intermediate(bool intermediate)
    {
        this->intermediate = intermediate;
    }

    auto WakeCalculatorOptions::SchemeMapper() const -> std::shared_ptr<WakeCategoryMapperInterface>
    {
        return schemeMapper;
    }

    void WakeCalculatorOptions::SchemeMapper(const std::shared_ptr<WakeCategoryMapperInterface> mapper)
    {
        schemeMapper = mapper;
    }
} // namespace UKControllerPlugin::Wake
