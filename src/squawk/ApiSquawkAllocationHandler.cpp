#pragma once
#include "pch/stdafx.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "squawk/ApiSquawkAllocationHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Squawk::ApiSquawkAllocation;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;

namespace UKControllerPlugin {
    namespace Squawk {

        ApiSquawkAllocationHandler::ApiSquawkAllocationHandler(EuroscopePluginLoopbackInterface & plugin)
            : plugin(plugin)
        {

        }

        /*
            Add a squawk event to the queue, but don't store duplicates
        */
        void ApiSquawkAllocationHandler::AddAllocationEvent(ApiSquawkAllocation event)
        {
            std::lock_guard<std::mutex> lock(this->queueGuard);
            this->eventQueue.insert(event);
        }

        /*
            Return the number of events in the queue
        */
        int ApiSquawkAllocationHandler::Count(void)
        {
            std::lock_guard<std::mutex> lock(this->queueGuard);
            return this->eventQueue.size();
        }

        /*
            Flush all items in the queue and assign the squawk
        */
        void ApiSquawkAllocationHandler::TimedEventTrigger(void)
        {
            std::lock_guard<std::mutex> lock(this->queueGuard);
            for (
                std::set<UKControllerPlugin::Squawk::ApiSquawkAllocation>::iterator it = this->eventQueue.begin();
                it != this->eventQueue.end();
            ) {
                try {
                    std::shared_ptr<EuroScopeCFlightPlanInterface> flightplan =
                        this->plugin.GetFlightplanForCallsign(it->callsign);

                    flightplan->SetSquawk(it->squawk);
                    LogInfo("Assigned squawk " + it->squawk + " to " + it->callsign);
                } catch (std::invalid_argument) {
                    // Flightplan has gone somewhere, do nothing
                    LogInfo("Could not find flightplan for " + it->callsign + " when trying to assign squawk");
                }

                this->eventQueue.erase(it++);
            }
        }
    }  // namespace Squawk
}  // namespace UKControllerPlugin
