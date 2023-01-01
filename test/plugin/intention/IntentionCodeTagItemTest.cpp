#include "intention/AircraftIntentionCode.h"
#include "intention/AircraftIntentionCodeGenerator.h"
#include "intention/IntentionCodeTagItem.h"
#include "tag/TagData.h"

using UKControllerPlugin::IntentionCode::AircraftIntentionCode;
using UKControllerPlugin::IntentionCode::AircraftIntentionCodeGenerator;
using UKControllerPlugin::IntentionCode::IntentionCodeTagItem;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPluginTest::IntentionCode {

    // Mock for the test
    class IntentionCodeTagItemTestMockGenerator : public AircraftIntentionCodeGenerator
    {
        public:
        IntentionCodeTagItemTestMockGenerator(std::shared_ptr<AircraftIntentionCode> returnCode)
            : returnCode(returnCode)
        {
        }

        auto Generate(
            const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
            -> std::shared_ptr<AircraftIntentionCode> override
        {
            return returnCode;
        }

        std::shared_ptr<AircraftIntentionCode> returnCode;
    };

    class IntentionCodeTagItemTest : public testing::Test
    {
        public:
        IntentionCodeTagItemTest()
            : generator(std::make_shared<IntentionCodeTagItemTestMockGenerator>(
                  std::shared_ptr<AircraftIntentionCode>(new AircraftIntentionCode))),
              tagItem(generator)
        {
            generator->returnCode->intentionCode = "FOO";
        }

        const double FONT_SIZE = 24.1;
        double fontSize = FONT_SIZE;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "Foooooo"; // NOLINT
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        std::shared_ptr<IntentionCodeTagItemTestMockGenerator> generator;
        IntentionCodeTagItem tagItem;
    };

    TEST_F(IntentionCodeTagItemTest, GetTagItemDescriptionReturnsCorrectDescription)
    {
        EXPECT_EQ("UKCP Intention Code", tagItem.GetTagItemDescription(0));
    }

    TEST_F(IntentionCodeTagItemTest, GetTagItemDataSetsTagItem)
    {
        TagData tagData(
            flightplan,
            radarTarget,
            1,
            EuroScopePlugIn::TAG_DATA_CORRELATED,
            itemString,
            &euroscopeColourCode,
            &tagColour,
            &fontSize);

        tagItem.SetTagItemData(tagData);
        EXPECT_EQ("FOO", tagData.GetItemString());
    }
} // namespace UKControllerPluginTest::IntentionCode
