#pragma once
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPluginTest {
    namespace Dependency {
        class MockDependencyLoader : public UKControllerPlugin::Dependency::DependencyLoaderInterface
        {
            public:
            MockDependencyLoader();
            virtual ~MockDependencyLoader();
            MOCK_METHOD(nlohmann::json, LoadDependency, (std::string, nlohmann::json), (noexcept));
        };
    } // namespace Dependency
} // namespace UKControllerPluginTest
