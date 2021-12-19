#include "message/UserMessager.h"
#include "metar/MetarComponents.h"
#include "metar/ParsedMetar.h"
#include "metar/ParsedMetarCollection.h"
#include "metar/PressureComponent.h"
#include "metar/PressureQueryCommandHandler.h"

using testing::_;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Metar::MetarComponents;
using UKControllerPlugin::Metar::ParsedMetar;
using UKControllerPlugin::Metar::ParsedMetarCollection;
using UKControllerPlugin::Metar::PressureComponent;
using UKControllerPlugin::Metar::PressureQueryCommandHandler;
using UKControllerPlugin::Metar::PressureUnit;

namespace UKControllerPluginTest::Metar {
    class PressureQueryCommandHandlerTest : public testing::Test
    {
        public:
        PressureQueryCommandHandlerTest() : messager(mockPlugin), handler(metars, messager)
        {
            this->metars.UpdateMetar(std::make_shared<ParsedMetar>("EGKK", "FOO", std::make_unique<MetarComponents>()));

            auto components = std::make_unique<MetarComponents>();
            components->pressure =
                std::make_shared<PressureComponent>(1013, 1011, 29.92, 29.84, PressureUnit::Hectopascals);
            this->metars.UpdateMetar(std::make_shared<ParsedMetar>("EGLL", "FOO", std::move(components)));
        }

        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        UserMessager messager;
        ParsedMetarCollection metars;
        PressureQueryCommandHandler handler;
    };

    TEST_F(PressureQueryCommandHandlerTest, ItReturnsFalseBadCommand)
    {
        EXPECT_FALSE(handler.ProcessCommand(".ukcp notpressure EGKK"));
    }

    TEST_F(PressureQueryCommandHandlerTest, ItReturnsFalseCommandWithThingsAfter)
    {
        EXPECT_FALSE(handler.ProcessCommand(".ukcp pressure EGKK lol"));
    }

    TEST_F(PressureQueryCommandHandlerTest, ItReturnsFalseCommandWithThingsBefore)
    {
        EXPECT_FALSE(handler.ProcessCommand(".ukcp lol pressure EGKK"));
    }

    TEST_F(PressureQueryCommandHandlerTest, ItReturnsFalseCommandAirfieldTooLong)
    {
        EXPECT_FALSE(handler.ProcessCommand(".ukcp pressure EGKKS"));
    }

    TEST_F(PressureQueryCommandHandlerTest, ItReturnsFalseCommandAirfieldTooShort)
    {
        EXPECT_FALSE(handler.ProcessCommand(".ukcp pressure EGK"));
    }

    TEST_F(PressureQueryCommandHandlerTest, ItReturnsFalseCommandNoAirfield)
    {
        EXPECT_FALSE(handler.ProcessCommand(".ukcp pressure"));
    }

    TEST_F(PressureQueryCommandHandlerTest, ItReturnsTrueAirfieldWithNoMetar)
    {
        EXPECT_TRUE(handler.ProcessCommand(".ukcp pressure EGXX"));
    }

    TEST_F(PressureQueryCommandHandlerTest, ItSendsMessageNoMetar)
    {
        EXPECT_CALL(
            this->mockPlugin, ChatAreaMessage(_, _, "Pressure information is not available for EGXX.", _, _, _, _, _))
            .Times(1);

        handler.ProcessCommand(".ukcp pressure EGXX");
    }

    TEST_F(PressureQueryCommandHandlerTest, ItReturnsTrueAirfieldWithNoPressureComponent)
    {
        EXPECT_TRUE(handler.ProcessCommand(".ukcp pressure EGLL"));
    }

    TEST_F(PressureQueryCommandHandlerTest, ItSendsMessageNoPressureComponent)
    {
        EXPECT_CALL(
            this->mockPlugin, ChatAreaMessage(_, _, "Pressure information is not available for EGKK.", _, _, _, _, _))
            .Times(1);

        handler.ProcessCommand(".ukcp pressure EGKK");
    }

    TEST_F(PressureQueryCommandHandlerTest, ItReturnsTrueAirfieldWithPressureComponent)
    {
        EXPECT_TRUE(handler.ProcessCommand(".ukcp pressure EGLL"));
    }

    TEST_F(PressureQueryCommandHandlerTest, ItSendsMessageWithPressureComponent)
    {
        EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, "QHH at EGLL is 1013, QFE is 1011.", _, _, _, _, _))
            .Times(1);

        handler.ProcessCommand(".ukcp pressure EGLL");
    }
} // namespace UKControllerPluginTest::Metar
