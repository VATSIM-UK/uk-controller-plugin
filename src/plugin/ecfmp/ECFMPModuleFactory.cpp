#include "ControllerFlowMeasureRelevance.h"
#include "ECFMPCustomMeasureFilterWrapper.h"
#include "ECFMPModuleFactory.h"
#include "HttpClient.h"
#include "Logger.h"

namespace UKControllerPlugin::ECFMP {

    struct ECFMPModuleFactory::Impl
    {
        [[nodiscard]] auto MakeSdk(Curl::CurlInterface& curl, const Controller::ActiveCallsignCollection& callsigns)
            -> std::shared_ptr<::ECFMP::Plugin::Sdk>
        {
            if (!sdk) {
                sdk = ::ECFMP::Plugin::SdkFactory::Build()
                          .WithLogger(std::make_unique<Logger>())
                          .WithHttpClient(std::make_unique<HttpClient>(curl))
                          .WithCustomFlowMeasureFilter(std::make_shared<ECFMPCustomMeasureFilterWrapper>(
                              std::make_shared<ControllerFlowMeasureRelevance>(callsigns)))
                          .Instance();
            }

            return sdk;
        }

        // The ECFMP SDK
        std::shared_ptr<::ECFMP::Plugin::Sdk> sdk;
    };

    ECFMPModuleFactory::ECFMPModuleFactory() : impl(std::make_unique<Impl>())
    {
        assert(this->impl && "ECFMPModuleFactory::impl is null");
    }

    ECFMPModuleFactory::~ECFMPModuleFactory()
    {
        impl->sdk->Destroy();
    };

    auto ECFMPModuleFactory::Sdk(Curl::CurlInterface& curl, const Controller::ActiveCallsignCollection& callsigns)
        -> std::shared_ptr<::ECFMP::Plugin::Sdk>
    {
        return impl->MakeSdk(curl, callsigns);
    }
} // namespace UKControllerPlugin::ECFMP
