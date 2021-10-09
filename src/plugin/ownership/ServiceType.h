#pragma once

namespace UKControllerPlugin::Ownership {
    enum class ServiceType
    {
        Delivery = 1,
        Ground = 2,
        Tower = 4,
        FinalApproach = 8,
        Approach = 16,
        Invalid = 32
    };
    
    inline ServiceType operator|(ServiceType first, ServiceType second)
    {
        return static_cast<ServiceType>(
            static_cast<unsigned int>(first) | static_cast<unsigned int>(second));
    }
    
    inline ServiceType operator&(ServiceType first, ServiceType second)
    {
        return static_cast<ServiceType>(
            static_cast<unsigned int>(first) & static_cast<unsigned int>(second));
    }
} // namespace UKControllerPlugin::Ownership
