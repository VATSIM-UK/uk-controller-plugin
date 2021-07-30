#include "pch/pch.h"
#include "prenote/PrenoteServiceFactory.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "prenote/PrenoteFactory.h"
#include "controller/ActiveCallsignCollection.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "airfield/AirfieldCollection.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "message/UserMessager.h"
#include "prenote/PrenoteService.h"
#include "bootstrap/BootstrapWarningMessage.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Prenote::PrenoteServiceFactory;
using UKControllerPlugin::Prenote::PrenoteFactory;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Prenote {

        class PrenoteserviceFactoryTest : public Test
        {
            public:
                void SetUp(void)
                {
                    this->collection = std::make_unique<ControllerPositionCollection>();
                    this->collection->AddPosition(std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(1, "EGKK_GND", 121.800, {"EGKK"}, true, false))
                    );
                    this->collection->AddPosition(std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(2, "EGKK_TWR", 124.220, {"EGKK"}, true, false))
                    );
                    this->hierarchyFactory = std::make_unique<ControllerPositionHierarchyFactory>(
                        *this->collection
                    );
                    this->prenoteFactory = std::make_unique<PrenoteFactory>(*this->hierarchyFactory);
                    this->airfieldOwnership = std::unique_ptr<AirfieldOwnershipManager>(
                        new AirfieldOwnershipManager(AirfieldCollection(), this->activeCallsigns)
                    );
                    this->userMessager = std::make_unique<UserMessager>(this->mockPlugin);
                    this->serviceFactory = std::make_unique<PrenoteServiceFactory>(
                        *this->prenoteFactory,
                        *this->userMessager
                    );
                };

                std::unique_ptr<ControllerPositionCollection> collection;
                std::unique_ptr<ControllerPositionHierarchyFactory> hierarchyFactory;
                std::unique_ptr<PrenoteFactory> prenoteFactory;
                std::unique_ptr<PrenoteServiceFactory> serviceFactory;
                std::unique_ptr<AirfieldOwnershipManager> airfieldOwnership;
                std::unique_ptr<UserMessager> userMessager;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                ActiveCallsignCollection activeCallsigns;
        };

        TEST_F(PrenoteserviceFactoryTest, CreateThrowsAnExceptionIfJsonNotArray)
        {
            nlohmann::json json = "";
            EXPECT_THROW(
                this->serviceFactory->Create(*this->airfieldOwnership, this->activeCallsigns, json),
                std::invalid_argument
            );
        }

        TEST_F(PrenoteserviceFactoryTest, CreateInformsUserIfJsonNotArray)
        {
            nlohmann::json json = "";

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    BootstrapWarningMessage::handler,
                    BootstrapWarningMessage::sender,
                    "Prenote data file not an array, prenotes not loaded",
                    true,
                    true,
                    true,
                    true,
                    true
                )
            )
            .Times(1);

            EXPECT_THROW(
                this->serviceFactory->Create(*this->airfieldOwnership, this->activeCallsigns, json),
                std::invalid_argument
            );
        }

        TEST_F(PrenoteserviceFactoryTest, DoesntAddPrenotesIfEmpty)
        {
            nlohmann::json json = nlohmann::json::parse("[]");
            EXPECT_THROW(
                this->serviceFactory->Create(*this->airfieldOwnership, this->activeCallsigns, json),
                std::invalid_argument
            );
        }

        TEST_F(PrenoteserviceFactoryTest, CreateInformsUserIfJsonArrayEmpty)
        {
            nlohmann::json json = nlohmann::json::parse("[]");

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    BootstrapWarningMessage::handler,
                    BootstrapWarningMessage::sender,
                    "Prenote data array empty, prenotes not loaded",
                    true,
                    true,
                    true,
                    true,
                    true
                ))
                .Times(1);

            EXPECT_THROW(
                this->serviceFactory->Create(*this->airfieldOwnership, this->activeCallsigns, json),
                std::invalid_argument
            );
        }

        TEST_F(PrenoteserviceFactoryTest, HandlesInvalidPrenotes)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["foo"] = 123;
            EXPECT_EQ(
                0,
                this->serviceFactory->Create(*this->airfieldOwnership, this->activeCallsigns, json)->CountPrenotes()
            );
        }

        TEST_F(PrenoteserviceFactoryTest, CreateInformsUserOfJsonException)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["foo"] = 123;

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    BootstrapWarningMessage::handler,
                    BootstrapWarningMessage::sender,
                    "JSON exception when loading prenotes: " + json.dump(),
                    true,
                    true,
                    true,
                    true,
                    true
                ))
                .Times(1);

            this->serviceFactory->Create(*this->airfieldOwnership, this->activeCallsigns, json);
        }

        TEST_F(PrenoteserviceFactoryTest, CreateInformsUserOfInvalidPrenotes)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["type"] = "sid";
            json[0]["airfield"] = 123;
            json[0]["departure"] = "BIG2X";
            json[0]["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    BootstrapWarningMessage::handler,
                    BootstrapWarningMessage::sender,
                    "Invalid prenote airfield",
                    true,
                    true,
                    true,
                    true,
                    true
                ))
                .Times(1);

            this->serviceFactory->Create(*this->airfieldOwnership, this->activeCallsigns, json);
        }

        TEST_F(PrenoteserviceFactoryTest, AddsValidPrenotes)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["type"] = "sid";
            json[0]["airfield"] = "EGKK";
            json[0]["departure"] = "BIG2X";
            json[0]["recipient"] = { "EGKK_GND", "EGKK_TWR" };
            EXPECT_EQ(
                1,
                this->serviceFactory->Create(*this->airfieldOwnership, this->activeCallsigns, json)->CountPrenotes()
            );
        }

    }  // namespace Prenote
}  // namespace UKControllerPluginTest
