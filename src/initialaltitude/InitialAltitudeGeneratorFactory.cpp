#include "pch/stdafx.h"
#include "initialaltitude/InitialAltitudeGeneratorFactory.h"
#include "initialaltitude/InitialAltitudeGenerator.h"
#include "dependency/DependencyCache.h"

using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator;
namespace UKControllerPlugin {
    namespace InitialAltitude {

        // The JSON file that will contain the dependencies we need for this module.
        static const std::string initialAltitudeFileDependency = "initial-altitudes.json";

        /*
            Creates an InitialAltitudeGenerator factory. Only includes IAs that are integers.
        */
        std::unique_ptr<InitialAltitudeGenerator> InitialAltitudeGeneratorFactory::Create(
            const DependencyCache & dependency
        ) {
            std::unique_ptr<InitialAltitudeGenerator> generator(new InitialAltitudeGenerator);
            nlohmann::json dependencyData;
            try {
                dependencyData = nlohmann::json::parse(dependency.GetDependency(initialAltitudeFileDependency));
            } catch (nlohmann::json::exception) {
                // If the JSON fails to parse, nothing we can do here, so just let the default value return.
                LogError("Found dependency data for initial altitudes, but the JSON was invalid");
                return generator;
            } catch (std::out_of_range) {
                LogError("Could not find dependency data for initial altitudes");
                return generator;
            }

            // Iterate over each airfield and each SID in turn
            int processed = 0;
            for (
                nlohmann::json::iterator itAirfield = dependencyData.begin();
                itAirfield != dependencyData.end();
                ++itAirfield
            ) {
                for (
                    nlohmann::json::iterator itSid = itAirfield.value().begin();
                    itSid != itAirfield.value().end();
                    ++itSid
                ) {

                    // Normalise the SID names, removing deprecation markings.
                    std::string sidName = (itSid.key()[0] == '#') ?
                        itSid.key().substr(1, itSid.key().size()) : itSid.key();

                    // We're only interested in cases where the value is an integer.
                    if (itSid.value().is_number_integer()) {
                        try {
                            generator->AddSid(itAirfield.key(), sidName, itSid.value());
                            processed++;
                        } catch (std::invalid_argument) {
                            // If we see a duplicate SID, then carry on.
                        }
                    } else {
                        LogError("Invalid SID altitude found for " + sidName);
                    }
                }
            }

            LogInfo("Added " + std::to_string(processed) + " initial altitudes");
            return generator;
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
