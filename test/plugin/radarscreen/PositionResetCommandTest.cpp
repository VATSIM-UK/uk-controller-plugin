#include "radarscreen/PositionResetCommand.h"
#include "radarscreen/RadarRenderableCollection.h"

using testing::NiceMock;
using testing::Test;
using UKControllerPlugin::RadarScreen::PositionResetCommand;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPluginTest::RadarScreen::MockRadarRenderableInterface;

namespace UKControllerPluginTest {
    namespace RadarScreen {

        class PositionResetCommandTest : public Test
        {
            public:
            PositionResetCommandTest() : command(collection)
            {
                mockRenderable.reset(new NiceMock<MockRadarRenderableInterface>);
                this->collection.RegisterRenderer(1, mockRenderable, this->collection.afterLists);
            }

            const std::string resetCommand = ".ukcp resetvisuals";
            std::shared_ptr<NiceMock<MockRadarRenderableInterface>> mockRenderable;
            RadarRenderableCollection collection;
            PositionResetCommand command;
        };

        TEST_F(PositionResetCommandTest, ProcessCommandReturnsFalseInvalidCommand)
        {
            EXPECT_FALSE(this->command.ProcessCommand("notacommand"));
        }

        TEST_F(PositionResetCommandTest, ProcessCommandReturnsTrueOnProcessedCommand)
        {
            EXPECT_TRUE(this->command.ProcessCommand(resetCommand));
        }

        TEST_F(PositionResetCommandTest, ProcessCommandResetsPositions)
        {
            EXPECT_CALL(*this->mockRenderable, ResetPosition()).Times(1);

            EXPECT_TRUE(this->command.ProcessCommand(resetCommand));
        }
    } // namespace RadarScreen
} // namespace UKControllerPluginTest
