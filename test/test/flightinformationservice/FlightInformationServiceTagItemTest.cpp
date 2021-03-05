#include "pch/pch.h"
#include "flightinformationservice/FlightInformationServiceTagItem.h"

using ::testing::Test;
using UKControllerPlugin::FlightInformationService::FlightInformationServiceTagItem;

namespace UKControllerPluginTest {
    namespace FlightInformationService {

        class FlightInformationServiceTagItemTest : public Test
        {
            public:
                FlightInformationServiceTagItem tagItem;
        };

        TEST_F(FlightInformationServiceTagItemTest, ItDescribesTheBaseTagItem)
        {
            EXPECT_EQ("Flight Information Service", tagItem.GetTagItemDescription(116));
        }

        TEST_F(FlightInformationServiceTagItemTest, ItDescribesTheBlankOptionTagItem)
        {
            EXPECT_EQ("Flight Information Service or Blank", tagItem.GetTagItemDescription(117));
        }
    }  // namespace FlightInformationService
}  // namespace UKControllerPluginTest
