#include "pch/pch.h"
#include "prenote/AbstractPrenote.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPluginTest {
    namespace Prenote {

        class ConcretePrenote : public AbstractPrenote
        {
            public:
                ConcretePrenote(
                    std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers
                ) : AbstractPrenote(std::move(controllers))
                {

                };

                bool IsApplicable(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                ) const override {
                    return false;
                }

                std::string GetSummaryString(void) const override
                {
                    return std::string();
                }
        };

        TEST(AbstractPrenote, GetControllersReturnsControllerHierarchy)
        {
            ControllerPosition position1("EGKK_DEL", 121.950, "DEL", { "EGKK" });
            ControllerPosition position2("EGKK_GND", 121.800, "GND", { "EGKK" });
            ControllerPosition position3("EGKK_TWR", 124.220, "TWR", { "EGKK" });
            std::unique_ptr<ControllerPositionHierarchy> hierarchy = std::make_unique<ControllerPositionHierarchy>();

            hierarchy->AddPosition(position1);
            hierarchy->AddPosition(position2);
            hierarchy->AddPosition(position3);

            // Take a copy of the hierarchy
            std::unique_ptr<ControllerPositionHierarchy> hierarchyCopy = std::make_unique<ControllerPositionHierarchy>(
                *hierarchy
            );
            ConcretePrenote prenote(std::move(hierarchy));

            // Check the hierarchies match
            ControllerPositionHierarchy returnedHierarchy = prenote.GetControllers();
            ControllerPositionHierarchy::const_iterator it1 = returnedHierarchy.cbegin();
            ControllerPositionHierarchy::const_iterator it2 = hierarchyCopy->cbegin();

            EXPECT_EQ(it1++->get(), it2++->get());
            EXPECT_EQ(it1++->get(), it2++->get());
            EXPECT_EQ(it1++->get(), it2++->get());
        }
    }  // namespace Prenote
}  // namespace UKControllerPluginTest
