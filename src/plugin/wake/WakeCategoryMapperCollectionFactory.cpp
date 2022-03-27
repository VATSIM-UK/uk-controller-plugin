#include "FlightplanWakeCategoryMapper.h"
#include "WakeCategoryMapperCollection.h"
#include "WakeCategoryMapperCollectionFactory.h"
#include "WakeScheme.h"
#include "WakeSchemeCollection.h"
#include "aircraft/AircraftTypeMapperInterface.h"

namespace UKControllerPlugin::Wake {

    auto MakeMapperCollection(
        const WakeSchemeCollection& collection, const Aircraft::AircraftTypeMapperInterface& aircraftTypeMapper)
        -> std::unique_ptr<WakeCategoryMapperCollection>
    {
        auto mapperCollection = std::make_unique<WakeCategoryMapperCollection>();
        collection.ForEach([&mapperCollection, &aircraftTypeMapper](const WakeScheme& scheme) {
            mapperCollection->Add(
                scheme.Id(), std::make_shared<FlightplanWakeCategoryMapper>(scheme, aircraftTypeMapper));
        });

        return mapperCollection;
    }
} // namespace UKControllerPlugin::Wake
