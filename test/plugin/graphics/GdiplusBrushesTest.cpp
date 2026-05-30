#include "graphics/GdiplusBrushes.h"
#include "graphics/Theme.h"

using UKControllerPlugin::Graphics::DEFAULT_THEME;
using UKControllerPlugin::Graphics::ITEC_THEME;
using UKControllerPlugin::Graphics::NERC_THEME;
using UKControllerPlugin::Graphics::NODE_THEME;
using UKControllerPlugin::Graphics::NOVA_THEME;
using UKControllerPlugin::Graphics::Theme;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPluginTest::Graphics {

    class GdiplusBrushesTest : public testing::Test
    {
        public:
        GdiplusBrushesTest() = default;
        GdiplusBrushes brushes;
    };

    TEST_F(GdiplusBrushesTest, DefaultConstructorLoadsDefaultTheme)
    {
        EXPECT_NE(nullptr, brushes.backgroundBrush);
        EXPECT_NE(nullptr, brushes.headerBrush);
        EXPECT_NE(nullptr, brushes.highlightedHeaderBrush);
        EXPECT_NE(nullptr, brushes.textBrush);
        EXPECT_NE(nullptr, brushes.highlightedTextBrush);
        EXPECT_NE(nullptr, brushes.mainAircraftTextBrush);
        EXPECT_NE(nullptr, brushes.highlightedAircraftTextBrush);
        EXPECT_NE(nullptr, brushes.timerGreenBrush);
        EXPECT_NE(nullptr, brushes.timerYellowBrush);
        EXPECT_NE(nullptr, brushes.timerRedBrush);
        EXPECT_NE(nullptr, brushes.borderPen);
        EXPECT_NE(nullptr, brushes.borderBrush);
        EXPECT_NE(nullptr, brushes.textPen);
    }

    TEST_F(GdiplusBrushesTest, LoadThemeReplacesBrushes)
    {
        // Save old brush
        auto oldHeaderBrush = brushes.headerBrush;

        // Load a different theme
        brushes.LoadTheme(NODE_THEME);

        // New brush should be different from old
        EXPECT_NE(oldHeaderBrush, brushes.headerBrush);
    }

    TEST_F(GdiplusBrushesTest, LoadThemeUpdatesCompatColorFields)
    {
        brushes.LoadTheme(NODE_THEME);

        EXPECT_EQ(0, brushes.background.GetR());
        EXPECT_EQ(0, brushes.background.GetG());
        EXPECT_EQ(0, brushes.background.GetB());

        EXPECT_EQ(255, brushes.text.GetR());
        EXPECT_EQ(255, brushes.text.GetG());
        EXPECT_EQ(255, brushes.text.GetB());
    }

    TEST_F(GdiplusBrushesTest, LoadThemeUpdatesStaticColorFields)
    {
        // euroscopeBackground and green should survive LoadTheme
        EXPECT_EQ(255, brushes.euroscopeBackground.GetA());
        EXPECT_EQ(255, brushes.green.GetA());
    }

    TEST_F(GdiplusBrushesTest, LoadThemeItecSetsCorrectBackground)
    {
        brushes.LoadTheme(ITEC_THEME);

        EXPECT_EQ(195, brushes.background.GetR());
        EXPECT_EQ(195, brushes.background.GetG());
        EXPECT_EQ(185, brushes.background.GetB());
    }

    TEST_F(GdiplusBrushesTest, ReloadingDefaultThemeProducesExpectedColors)
    {
        brushes.LoadTheme(DEFAULT_THEME);

        EXPECT_EQ(58, brushes.background.GetR());
        EXPECT_EQ(57, brushes.background.GetG());
        EXPECT_EQ(58, brushes.background.GetB());
    }

    TEST_F(GdiplusBrushesTest, LoadThemeNercSetsCorrectBorder)
    {
        brushes.LoadTheme(NERC_THEME);

        EXPECT_EQ(83, brushes.border.GetR());
        EXPECT_EQ(83, brushes.border.GetG());
        EXPECT_EQ(83, brushes.border.GetB());
    }

    TEST_F(GdiplusBrushesTest, LoadThemeNovaSetsCorrectHeaders)
    {
        brushes.LoadTheme(NOVA_THEME);

        EXPECT_EQ(0, brushes.header.GetR());
        EXPECT_EQ(140, brushes.header.GetG());
        EXPECT_EQ(200, brushes.header.GetB());
    }

    TEST_F(GdiplusBrushesTest, BorderBrushIsNotNullAfterLoadTheme)
    {
        brushes.LoadTheme(DEFAULT_THEME);
        EXPECT_NE(nullptr, brushes.borderBrush);
    }

    TEST_F(GdiplusBrushesTest, TextPenIsNotNullAfterLoadTheme)
    {
        brushes.LoadTheme(NODE_THEME);
        EXPECT_NE(nullptr, brushes.textPen);
    }
} // namespace UKControllerPluginTest::Graphics
