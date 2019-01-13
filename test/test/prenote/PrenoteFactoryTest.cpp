#include "pch/pch.h"
#include "prenote/PrenoteFactory.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerPosition.h"
#include "prenote/DeparturePrenote.h"
#include "prenote/AbstractPrenote.h"
#include "prenote/AirfieldPairingPrenote.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Prenote::PrenoteFactory;
using UKControllerPlugin::Prenote::DeparturePrenote;
using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Prenote::AirfieldPairingPrenote;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Prenote {

        class PrenoteFactoryTest : public Test
        {
            public:
                void SetUp(void)
                {
                    this->collection = std::make_unique<ControllerPositionCollection>();
                    this->collection->AddPosition(std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGKK_GND", 121.800, "GND", { "EGKK" }))
                    );
                    this->collection->AddPosition(std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGKK_TWR", 124.220, "TWR", { "EGKK" }))
                    );
                    this->hierarchyFactory = std::make_unique<ControllerPositionHierarchyFactory>(
                        *this->collection
                    );
                };

                std::unique_ptr<ControllerPositionCollection> collection;
                std::unique_ptr<ControllerPositionHierarchyFactory> hierarchyFactory;
        };

        TEST_F(PrenoteFactoryTest, CreateFromJsonThrowsExceptionIfJsonNotObject)
        {
            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(""), std::invalid_argument);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonThrowsExceptionIfTypeFieldNotString)
        {
            nlohmann::json json;
            json["type"] = 123;

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonThrowsExceptionIfTypeFieldNotKnown)
        {
            nlohmann::json json;
            json["type"] = "something";

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonSidThrowsJsonExceptionIfAirfieldField)
        {
            nlohmann::json json;
            json["type"] = "sid";
            json["departure"] = "BIG2X";
            json["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), nlohmann::json::exception);
        }


        TEST_F(PrenoteFactoryTest, CreateFromJsonSidThrowsExceptionIfAirfieldNotString)
        {
            nlohmann::json json;
            json["type"] = "sid";
            json["airfield"] = 123;
            json["departure"] = "BIG2X";

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonSidThrowsJsonExceptionIfNoDepartureField)
        {
            nlohmann::json json;
            json["type"] = "sid";
            json["airfield"] = "EGKK";
            json["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), nlohmann::json::exception);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonSidThrowsExceptionIfDepartureNotString)
        {
            nlohmann::json json;
            json["type"] = "sid";
            json["airfield"] = "EGKK";
            json["departure"] = 123;

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonSidThrowsJsonExceptionIfNoReceipientField)
        {
            nlohmann::json json;
            json["type"] = "sid";
            json["airfield"] = "EGKK";
            json["departure"] = "BIG2X";

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), nlohmann::json::exception);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonSidThrowsExceptionIfProblemWithHierarchy)
        {
            nlohmann::json json;
            json["type"] = "sid";
            json["airfield"] = "EGKK";
            json["departure"] = "BIG2X";
            json["recipient"] = { "EGKK_GND", 123 };

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonSidReturnsADeparturePrenote)
        {
            nlohmann::json json;
            json["type"] = "sid";
            json["airfield"] = "EGKK";
            json["departure"] = "BIG2X";
            json["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            PrenoteFactory factory(*this->hierarchyFactory);
            std::unique_ptr<AbstractPrenote> prenote = factory.CreateFromJson(json);

            DeparturePrenote * departure = reinterpret_cast<DeparturePrenote *>(prenote.get());

            EXPECT_TRUE("EGKK, BIG2X" == departure->GetSummaryString());

            ControllerPositionHierarchy::const_iterator it = departure->GetControllers().cbegin();
            EXPECT_EQ(this->collection->FetchPositionByCallsign("EGKK_GND"), *it++);
            EXPECT_EQ(this->collection->FetchPositionByCallsign("EGKK_TWR"), *it++);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonPairingThrowsExceptionIfOriginNotString)
        {
            nlohmann::json json;
            json["type"] = "airfieldPairing";
            json["origin"] = 123;
            json["destination"] = "EGHI";
            json["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonPairingThrowsJsonExceptionIfNoOriginField)
        {
            nlohmann::json json;
            json["type"] = "airfieldPairing";
            json["destination"] = "EGHI";
            json["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), nlohmann::json::exception);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonPairingThrowsJsonExceptionIfNoDestinationField)
        {
            nlohmann::json json;
            json["type"] = "airfieldPairing";
            json["origin"] = "EGKK";
            json["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), nlohmann::json::exception);
        }


        TEST_F(PrenoteFactoryTest, CreateFromJsonPairingThrowsExceptionIfDestinationNotString)
        {
            nlohmann::json json;
            json["type"] = "airfieldPairing";
            json["origin"] = "EGKK";
            json["destination"] = 123;
            json["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonPairingThrowsJsonExceptionIfNoReceipientField)
        {
            nlohmann::json json;
            json["type"] = "airfieldPairing";
            json["origin"] = "EGKK";
            json["destination"] = "EGHI";

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), nlohmann::json::exception);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonPairingThrowsExceptionIfProblemWithHierarchy)
        {
            nlohmann::json json;
            json["type"] = "airfieldPairing";
            json["origin"] = "EGKK";
            json["destination"] = "EGHI";
            json["recipient"] = { "EGKK_GND", "EGKK_TWR", 123};

            PrenoteFactory factory(*this->hierarchyFactory);
            EXPECT_THROW(factory.CreateFromJson(json), std::invalid_argument);
        }

        TEST_F(PrenoteFactoryTest, CreateFromJsonPairingReturnsAPairingPrenote)
        {
            nlohmann::json json;
            json["type"] = "airfieldPairing";
            json["origin"] = "EGKK";
            json["destination"] = "EGHI";
            json["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            PrenoteFactory factory(*this->hierarchyFactory);
            std::unique_ptr<AbstractPrenote> prenote = factory.CreateFromJson(json);

            AirfieldPairingPrenote * pairing = reinterpret_cast<AirfieldPairingPrenote *>(prenote.get());

            EXPECT_TRUE("EGKK -> EGHI" == pairing->GetSummaryString());

            ControllerPositionHierarchy::const_iterator it = pairing->GetControllers().cbegin();
            EXPECT_EQ(this->collection->FetchPositionByCallsign("EGKK_GND"), *it++);
            EXPECT_EQ(this->collection->FetchPositionByCallsign("EGKK_TWR"), *it++);
        }
    }  // namespace Prenote
}  // namespace UKControllerPluginTest
