#include "pch/pch.h"
#include "prenote/AbstractPrenote.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Prenote::AbstractPrenote;

namespace UKControllerPluginTest {
    namespace Prenote {

        class ConcretePrenote : public AbstractPrenote
        {
            public:
            ConcretePrenote(std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers)
                : AbstractPrenote(std::move(controllers)){

                  };

            bool
            IsApplicable(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan) const override
            {
                return false;
            }

            std::string GetSummaryString(void) const override
            {
                return std::string();
            }
        };

        TEST(AbstractPrenote, GetControllersReturnsControllerHierarchy)
        {
            auto position1 = std::make_shared<ControllerPosition>(
                1, "EGKK_DEL", 121.950, std::vector<std::string>{"EGKK"}, true, false);
            auto position2 = std::make_shared<ControllerPosition>(
                2, "EGKK_GND", 121.800, std::vector<std::string>{"EGKK"}, true, false);
            auto position3 = std::make_shared<ControllerPosition>(
                3, "EGKK_TWR", 124.220, std::vector<std::string>{"EGKK"}, true, false);
            std::unique_ptr<ControllerPositionHierarchy> hierarchy = std::make_unique<ControllerPositionHierarchy>();

            hierarchy->AddPosition(position1);
            hierarchy->AddPosition(position2);
            hierarchy->AddPosition(position3);

            // Take a copy of the hierarchy
            std::unique_ptr<ControllerPositionHierarchy> hierarchyCopy =
                std::make_unique<ControllerPositionHierarchy>(*hierarchy);
            ConcretePrenote prenote(std::move(hierarchy));

            // Check the hierarchies match
            ControllerPositionHierarchy returnedHierarchy = prenote.GetControllers();
            ControllerPositionHierarchy::const_iterator it1 = returnedHierarchy.cbegin();
            ControllerPositionHierarchy::const_iterator it2 = hierarchyCopy->cbegin();

            EXPECT_EQ(it1++->get(), it2++->get());
            EXPECT_EQ(it1++->get(), it2++->get());
            EXPECT_EQ(it1++->get(), it2++->get());
        }
    } // namespace Prenote
} // namespace UKControllerPluginTest
