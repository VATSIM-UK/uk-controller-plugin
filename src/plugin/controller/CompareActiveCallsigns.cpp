#include "controller/CompareActiveCallsigns.h"
#include "controller/ActiveCallsign.h"

namespace UKControllerPlugin::Controller {
    bool CompareActiveCallsigns::operator()(
        const std::shared_ptr<ActiveCallsign>& callsign, std::string callsignString) const
    {
        return callsign->GetCallsign() < callsignString;
    }

    bool CompareActiveCallsigns::operator()(
        std::string callsignString, const std::shared_ptr<ActiveCallsign>& callsign) const
    {
        return callsignString < callsign->GetCallsign();
    }

    bool CompareActiveCallsigns::operator()(
        const std::shared_ptr<ActiveCallsign>& a, const std::shared_ptr<ActiveCallsign>& b) const
    {
        return a->GetCallsign() < b->GetCallsign();
    }
} // namespace UKControllerPlugin::Controller
