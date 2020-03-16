#include "pch/stdafx.h"
#include "initialaltitude/InitialAltitudeGeneratorFactory.h"
#include "initialaltitude/InitialAltitudeGenerator.h"
#include "dependency/DependencyLoaderInterface.h"

using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator;
namespace UKControllerPlugin {
    namespace InitialAltitude {

        // The JSON file that will contain the dependencies we need for this module.
        static const std::string initialAltitudeFileDependency = "DEPENDENCY_INITIAL_ALTITUDES";

        // The default dependency value
        static const nlohmann::json sidsDependencyDefault = nlohmann::json::array();

        /*
            Creates an InitialAltitudeGenerator factory. Only includes IAs that are integers.
        */
        std::unique_ptr<InitialAltitudeGenerator> InitialAltitudeGeneratorFactory::Create(
            DependencyLoaderInterface & dependency
        ) {
            std::unique_ptr<InitialAltitudeGenerator> generator(new InitialAltitudeGenerator);
            nlohmann::json dependencyData = dependency.LoadDependency(
                initialAltitudeFileDependency,
                sidsDependencyDefault
            );

            if (dependencyData == sidsDependencyDefault) {
                LogError("Initial altitude dependency not found");
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
