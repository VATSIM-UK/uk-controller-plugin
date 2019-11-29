#pragma once
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPluginTest {
    namespace Dependency {
        class MockDependencyLoader : public UKControllerPlugin::Dependency::DependencyLoaderInterface
        {
            public:
                MOCK_METHOD2(LoadDependency, nlohmann::json(std::string, nlohmann::json));
        };
    }  // namespace Update
}  // namespace UKControllerPluginTest
