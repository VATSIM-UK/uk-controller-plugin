#include "pch/pch.h"
#include "intention/IntentionCodeFactory.h"
#include "intention/AmsterdamAirfieldGroup.h"
#include "intention/BrusselsAirfieldGroup.h"
#include "intention/DublinAirfieldGroup.h"
#include "intention/HomeAirfieldGroup.h"
#include "intention/ShannonAirfieldGroup.h"
#include "intention/IntentionCodeGenerator.h"
#include "intention/SectorExitRepository.h"

using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::IntentionCode::SectorExitRepository;

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Adds the airfield groups to the intention code generator.
        */
        std::unique_ptr<IntentionCodeGenerator> IntentionCodeFactory::Create(SectorExitRepository & exitPoints)
        {
            std::vector <std::unique_ptr<AirfieldGroup>> airfields;
            airfields.push_back(std::unique_ptr<AirfieldGroup>(new HomeAirfieldGroup));
            airfields.push_back(std::unique_ptr<AirfieldGroup>(new ShannonAirfieldGroup));
            airfields.push_back(std::unique_ptr<AirfieldGroup>(new BrusselsAirfieldGroup));
            airfields.push_back(std::unique_ptr<AirfieldGroup>(new DublinAirfieldGroup));
            airfields.push_back(std::unique_ptr<AirfieldGroup>(new AmsterdamAirfieldGroup));

            return std::unique_ptr<IntentionCodeGenerator>(
                new IntentionCodeGenerator(std::move(airfields), exitPoints)
            );
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
