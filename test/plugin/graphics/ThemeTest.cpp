#include "graphics/Theme.h"

using UKControllerPlugin::Graphics::DEFAULT_THEME;
using UKControllerPlugin::Graphics::ITEC_THEME;
using UKControllerPlugin::Graphics::NERC_THEME;
using UKControllerPlugin::Graphics::NODE_THEME;
using UKControllerPlugin::Graphics::NOVA_THEME;

namespace UKControllerPluginTest::Graphics {

    class ThemeTest : public testing::Test
    {
    };

    TEST_F(ThemeTest, DefaultThemeHasCorrectBackground)
    {
        EXPECT_EQ(58, DEFAULT_THEME.background.GetR());
        EXPECT_EQ(57, DEFAULT_THEME.background.GetG());
        EXPECT_EQ(58, DEFAULT_THEME.background.GetB());
    }

    TEST_F(ThemeTest, DefaultThemeHasCorrectText)
    {
        EXPECT_EQ(227, DEFAULT_THEME.text.GetR());
        EXPECT_EQ(227, DEFAULT_THEME.text.GetG());
        EXPECT_EQ(227, DEFAULT_THEME.text.GetB());
    }

    TEST_F(ThemeTest, NodeThemeHasCorrectBackground)
    {
        EXPECT_EQ(0, NODE_THEME.background.GetR());
        EXPECT_EQ(0, NODE_THEME.background.GetG());
        EXPECT_EQ(0, NODE_THEME.background.GetB());
    }

    TEST_F(ThemeTest, NodeThemeHasCorrectHeader)
    {
        EXPECT_EQ(160, NODE_THEME.header.GetR());
        EXPECT_EQ(111, NODE_THEME.header.GetG());
        EXPECT_EQ(112, NODE_THEME.header.GetB());
    }

    TEST_F(ThemeTest, NercThemeHasCorrectBackground)
    {
        EXPECT_EQ(150, NERC_THEME.background.GetR());
        EXPECT_EQ(150, NERC_THEME.background.GetG());
        EXPECT_EQ(150, NERC_THEME.background.GetB());
    }

    TEST_F(ThemeTest, NercThemeHasCorrectBorder)
    {
        EXPECT_EQ(83, NERC_THEME.border.GetR());
        EXPECT_EQ(83, NERC_THEME.border.GetG());
        EXPECT_EQ(83, NERC_THEME.border.GetB());
    }

    TEST_F(ThemeTest, NovaThemeHasCorrectBackground)
    {
        EXPECT_EQ(180, NOVA_THEME.background.GetR());
        EXPECT_EQ(180, NOVA_THEME.background.GetG());
        EXPECT_EQ(180, NOVA_THEME.background.GetB());
    }

    TEST_F(ThemeTest, NovaThemeHasCorrectHeader)
    {
        EXPECT_EQ(0, NOVA_THEME.header.GetR());
        EXPECT_EQ(140, NOVA_THEME.header.GetG());
        EXPECT_EQ(200, NOVA_THEME.header.GetB());
    }

    TEST_F(ThemeTest, ItecThemeHasCorrectBackground)
    {
        EXPECT_EQ(195, ITEC_THEME.background.GetR());
        EXPECT_EQ(195, ITEC_THEME.background.GetG());
        EXPECT_EQ(185, ITEC_THEME.background.GetB());
    }

    TEST_F(ThemeTest, ItecThemeHasCorrectBorder)
    {
        EXPECT_EQ(0, ITEC_THEME.border.GetR());
        EXPECT_EQ(0, ITEC_THEME.border.GetG());
        EXPECT_EQ(0, ITEC_THEME.border.GetB());
    }

    TEST_F(ThemeTest, NodeTimerGreenIsCorrect)
    {
        EXPECT_EQ(104, NODE_THEME.timerGreen.GetR());
        EXPECT_EQ(221, NODE_THEME.timerGreen.GetG());
        EXPECT_EQ(69, NODE_THEME.timerGreen.GetB());
    }

    TEST_F(ThemeTest, NercHighlightedTextIsCorrect)
    {
        EXPECT_EQ(255, NERC_THEME.highlightedText.GetR());
        EXPECT_EQ(255, NERC_THEME.highlightedText.GetG());
        EXPECT_EQ(255, NERC_THEME.highlightedText.GetB());
    }
} // namespace UKControllerPluginTest::Graphics
