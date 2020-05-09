#include "pch/pch.h.h"
#include "##FOLDERNAME##/##CLASSNAME##.h"

using ::Testing::Test;

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
