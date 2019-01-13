#pragma once
#include "DependencyLoader.h"

namespace UKControllerPluginTest {
    namespace Update {
        class MockDependencyLoader : public UKControllerPlugin::Update::DependencyLoader
        {
            public:
                MockDependencyLoader() : DependencyLoader(NULL, NULL) {}
                MOCK_CONST_METHOD1(GetDependency, std::string(std::string));
        };
    }  // namespace Update
}  // namespace UKControllerPluginTest
