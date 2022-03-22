#include "mapping/MappingElement.h"

using UKControllerPlugin::Mapping::DisplayRule;
using UKControllerPlugin::Mapping::MappingElement;

namespace UKControllerPluginTest::Mapping {
    class MappingElementTest : public testing::Test
    {
        public:
        MappingElementTest()
            : displayRule1(std::make_shared<testing::NiceMock<Mapping::MockDisplayRule>>()),
              displayRule2(std::make_shared<testing::NiceMock<Mapping::MockDisplayRule>>()),
              displayRules({displayRule1, displayRule2}),
              drawer(std::make_shared<testing::NiceMock<Mapping::MockMappingElementDrawer>>()),
              element(1, "visual_reference_point", "M5 Avon Bridge", Position(), displayRules, drawer)
        {
        }

        [[nodiscard]] static auto Position() -> EuroScopePlugIn::CPosition
        {
            EuroScopePlugIn::CPosition position;
            position.m_Latitude = 1;
            position.m_Longitude = 2;

            return position;
        }

        testing::NiceMock<Windows::MockGraphicsInterface> graphics;
        std::shared_ptr<testing::NiceMock<Mapping::MockDisplayRule>> displayRule1;
        std::shared_ptr<testing::NiceMock<Mapping::MockDisplayRule>> displayRule2;
        std::set<std::shared_ptr<DisplayRule>> displayRules;
        std::shared_ptr<testing::NiceMock<Mapping::MockMappingElementDrawer>> drawer;
        MappingElement element;
    };

    TEST_F(MappingElementTest, ItHasAnId)
    {
        EXPECT_EQ(1, element.Id());
    }

    TEST_F(MappingElementTest, ItHasAType)
    {
        EXPECT_EQ("visual_reference_point", element.Type());
    }

    TEST_F(MappingElementTest, ItHasALabel)
    {
        EXPECT_EQ("M5 Avon Bridge", element.Label());
    }

    TEST_F(MappingElementTest, ItHasAPosition)
    {
        const auto position = element.Position();
        EXPECT_EQ(1, position.m_Latitude);
        EXPECT_EQ(2, position.m_Longitude);
    }

    TEST_F(MappingElementTest, ItShouldDisplayIfAllRulesPass)
    {
        ON_CALL(*displayRule1, Passes()).WillByDefault(testing::Return(true));

        ON_CALL(*displayRule2, Passes()).WillByDefault(testing::Return(true));

        EXPECT_TRUE(element.ShouldDisplay());
    }

    TEST_F(MappingElementTest, ItShouldNotDisplayIfARuleDoesNotPass)
    {
        ON_CALL(*displayRule1, Passes()).WillByDefault(testing::Return(true));

        ON_CALL(*displayRule2, Passes()).WillByDefault(testing::Return(false));

        EXPECT_FALSE(element.ShouldDisplay());
    }

    TEST_F(MappingElementTest, ItDraws)
    {
        EXPECT_CALL(*drawer, Draw(testing::_, testing::_)).Times(1);

        Gdiplus::Rect rect{1, 2, 3, 4};
        element.Draw(graphics, rect);
    }
} // namespace UKControllerPluginTest::Mapping
