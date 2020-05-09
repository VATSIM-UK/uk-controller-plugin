#include "pch/pch.h"
#include "##FOLDERNAME##/##CLASSNAME##.h"

using ::testing::Test;

namespace UKControllerPluginTest {
    namespace ##NAMESPACE## {

        class ##CLASSNAME##Test : public Test
        {

        }

        TEST_F(##CLASSNAME##Test, ItDoesSomething)
        {
            EXPECT_TRUE(true);
        }
    }  // namespace ##NAMESPACE##
}  // namespace UKControllerPluginTest
