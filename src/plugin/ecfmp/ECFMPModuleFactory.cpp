#include "ECFMPModuleFactory.h"
#include "HttpClient.h"
#include "Logger.h"

namespace UKControllerPlugin::ECFMP {

    struct ECFMPModuleFactory::Impl
    {
        [[nodiscard]] auto MakeSdk(Curl::CurlInterface& curl) -> std::shared_ptr<::ECFMP::Plugin::Sdk>
        {
            if (!sdk) {
                sdk = ::ECFMP::Plugin::SdkFactory::Build()
                          .WithLogger(std::make_unique<Logger>())
                          .WithHttpClient(std::make_unique<HttpClient>(curl))
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

    auto ECFMPModuleFactory::Sdk(Curl::CurlInterface& curl) -> std::shared_ptr<::ECFMP::Plugin::Sdk>
    {
        return impl->MakeSdk(curl);
    }
} // namespace UKControllerPlugin::ECFMP
