#pragma once
#include "pch/pch.h"


class TestEnvironment : public ::testing::Environment
{
    public:

        virtual ~TestEnvironment() {}

        virtual void SetUp()
        {
            // Put in a null logger
            SetLoggerInstance(
                std::make_shared<spdlog::logger>(
                    "null_logger",
                    std::make_shared<spdlog::sinks::null_sink_mt>()
                )
            );

            // Start up GDI so we can use its functions, even though not drawing to screen.
            Gdiplus::GdiplusStartupInput gdiStartup;
            Gdiplus::GdiplusStartup(&this->gdiPlusToken, &gdiStartup, NULL);
        }

        virtual void TearDown()
        {
            Gdiplus::GdiplusShutdown(this->gdiPlusToken);
        }

        ULONG_PTR gdiPlusToken;
};
