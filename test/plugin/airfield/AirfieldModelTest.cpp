#include "pch/pch.h"
#include "airfield/AirfieldModel.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using ::testing::ElementsAre;

namespace UKControllerPluginTest {
    namespace AirfieldOwnership {

        TEST(AirfieldModel, ConstructorThrowsExceptionIfIcaoTooLong)
        {
            EXPECT_THROW(AirfieldModel("EGXYZ", {}), std::invalid_argument);
        }

        TEST(AirfieldModel, ConstructorThrowsExceptionIfIcaoNotUk)
        {
            EXPECT_THROW(AirfieldModel("LIML", {}), std::invalid_argument);
        }

        TEST(AirfieldModel, GetIcaoReturnsIcao)
        {
            std::string icao = "EGKK";
            AirfieldModel airfield(icao, {});
            EXPECT_EQ(0, icao.compare(airfield.GetIcao()));
        }

        TEST(AirfieldModel, GetOwnershipPresedenceReturnsOwnership)
        {
            AirfieldModel airfield("EGKK", { "EGKK_DEL", "EGKK_GND" });
            EXPECT_THAT(airfield.GetOwnershipPresedence(), ElementsAre("EGKK_DEL", "EGKK_GND"));
        }

        TEST(AirfieldModel, EqualityOperatorReturnsFalseDifferentIcao)
        {
            AirfieldModel airfield("EGKK", { "EGKK_DEL", "EGKK_GND" });
            AirfieldModel airfield2("EGLL", { "EGKK_DEL", "EGKK_GND" });
            EXPECT_FALSE(airfield == airfield2);
        }

        TEST(AirfieldModel, EqualityOperatorReturnsTrueSameIcao)
        {
            AirfieldModel airfield("EGKK", { "EGKK_DEL", "EGKK_GND" });
            AirfieldModel airfield2("EGKK", { "EGKK_DEL", "EGKK_GND" });
            EXPECT_TRUE(airfield == airfield2);
        }
    }  // namespace AirfieldOwnership
}  // namespace UKControllerPluginTest
