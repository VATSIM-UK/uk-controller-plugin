#include "pch/pch.h"
#include "dependency/DependencyProviderInterface.h"
#include "dependency/DependencyData.h"

namespace UKControllerPluginTest {
    namespace Dependency {
        
        class MockDependencyProvider : public UKControllerPlugin::Dependency::DependencyProviderInterface
        {
            public:
                std::string GetProviderType(void) const override 
                {
                    return "MockDependencyProvider";
                }

                MOCK_CONST_METHOD1(GetDependency, nlohmann::json(UKControllerPlugin::Dependency::DependencyData));
        };

    }  // namespace Dependency
}  // namespace UKControllerPluginTest
