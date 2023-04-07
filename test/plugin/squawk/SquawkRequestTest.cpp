#include "squawk/SquawkRequest.h"

using ::testing::Test;
using UKControllerPlugin::Squawk::SquawkRequest;

namespace UKControllerPluginTest {
    namespace Squawk {

        class SquawkRequestTest : public Test
        {
            public:
            SquawkRequest request;
        };

        TEST_F(SquawkRequestTest, TestItReturnsTrueOnSuccessfulStart)
        {
            EXPECT_TRUE(this->request.Start("BAW123"));
        }

        TEST_F(SquawkRequestTest, TestItReturnsFalseOnDuplicateRequest)
        {
            EXPECT_TRUE(this->request.Start("BAW123"));
            EXPECT_FALSE(this->request.Start("BAW123"));
        }

        TEST_F(SquawkRequestTest, TestItAllowsMultipleRequestsDifferentCallsign)
        {
            EXPECT_TRUE(this->request.Start("BAW123"));
            EXPECT_TRUE(this->request.Start("EZY465"));
        }

        TEST_F(SquawkRequestTest, TestNewRequestAllowedOncePreviousEnds)
        {
            EXPECT_TRUE(this->request.Start("BAW123"));
            this->request.End("BAW123");
            EXPECT_TRUE(this->request.Start("BAW123"));
        }

        TEST_F(SquawkRequestTest, TestEndHandlesNonExistantRequestsGracefully)
        {
            EXPECT_NO_THROW(this->request.End("BAW123"));
        }

    } // namespace Squawk
} // namespace UKControllerPluginTest
