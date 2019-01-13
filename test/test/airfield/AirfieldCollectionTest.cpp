#include "pch/pch.h"
#include "airfield/Airfield.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Airfield::Airfield;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPluginTest {
    namespace AirfieldOwnership {

        TEST(AirfieldCollection, AddAirfieldThrowsExceptionIfAlreadyAdded)
        {
            AirfieldCollection collection;
            collection.AddAirfield(std::unique_ptr<Airfield>(new Airfield("EGKK", {})));
            EXPECT_THROW(
                collection.AddAirfield(std::unique_ptr<Airfield> (new Airfield("EGKK", { "EGKK_DEL" }))),
                std::invalid_argument
            );
        }

        TEST(AirfieldCollection, FetchAirfieldByIcaoThrowsExceptionIfNotUkAirfield)
        {
            AirfieldCollection collection;
            EXPECT_THROW(collection.FetchAirfieldByIcao("LFPG"), std::out_of_range);
        }

        TEST(AirfieldCollection, FetchAirfieldByIcaoThrowsExceptionIfNoAirfields)
        {
            AirfieldCollection collection;
            EXPECT_THROW(collection.FetchAirfieldByIcao("EGLL"), std::out_of_range);
        }

        TEST(AirfieldCollection, FetchAirfieldByIcaoThrowsExceptionIfNotFound)
        {
            AirfieldCollection collection;
            collection.AddAirfield(std::unique_ptr<Airfield>(new Airfield("EGKK", {})));
            EXPECT_THROW(collection.FetchAirfieldByIcao("EGLL"), std::out_of_range);
        }

        TEST(AirfieldCollection, FetchAirfieldByIcaoReturnsAirfieldIfFound)
        {
            AirfieldCollection collection;
            std::unique_ptr<Airfield> airfield(new Airfield("EGKK", {}));
            Airfield * airfieldRaw = airfield.get();

            collection.AddAirfield(std::move(airfield));
            EXPECT_TRUE(*airfieldRaw == collection.FetchAirfieldByIcao("EGKK"));
        }

        TEST(AirfieldCollection, GetSizeReturnsNumberOfItemsInCollection)
        {
            AirfieldCollection collection;
            EXPECT_EQ(0, collection.GetSize());
            collection.AddAirfield(std::unique_ptr<Airfield> (new Airfield("EGKK", {})));
            collection.AddAirfield(std::unique_ptr<Airfield> (new Airfield("EGLL", {})));
            EXPECT_EQ(2, collection.GetSize());
        }
    }  // namespace AirfieldOwnership
}  // namespace UKControllerPluginTest
