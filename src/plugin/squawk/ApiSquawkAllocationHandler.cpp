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
        void ApiSquawkAllocationHandler::AddAllocationToQueue(ApiSquawkAllocation event)
        {
            std::lock_guard<std::mutex> lock(this->queueGuard);
            this->allocationQueue.insert(event);
        }

        /*
            Return the number of events in the queue
        */
        int ApiSquawkAllocationHandler::Count(void)
        {
            std::lock_guard<std::mutex> lock(this->queueGuard);
            return this->allocationQueue.size();
        }

        /*
            Returns the first allocation on the queue
        */
        UKControllerPlugin::Squawk::ApiSquawkAllocation ApiSquawkAllocationHandler::First(void) const
        {
            return this->allocationQueue.size() > 0 ? *this->allocationQueue.cbegin() : this->invalid;
        }

        /*
            Flush all items in the queue and assign the squawk
        */
        void ApiSquawkAllocationHandler::TimedEventTrigger(void)
        {
            std::lock_guard<std::mutex> lock(this->queueGuard);
            std::shared_ptr<EuroScopeCFlightPlanInterface> flightplan;
            for (
                std::set<UKControllerPlugin::Squawk::ApiSquawkAllocation>::iterator it = this->allocationQueue.begin();
                it != this->allocationQueue.end();
            ) {
                flightplan = this->plugin.GetFlightplanForCallsign(it->callsign);
                if (!flightplan) {
                    LogInfo("Could not find flightplan for " + it->callsign + " when trying to assign squawk");
                } else {
                    flightplan->SetSquawk(it->squawk);
                    LogInfo("Assigned squawk " + it->squawk + " to " + it->callsign);
                }

                this->allocationQueue.erase(it++);
            }
        }
    }  // namespace Squawk
}  // namespace UKControllerPlugin
