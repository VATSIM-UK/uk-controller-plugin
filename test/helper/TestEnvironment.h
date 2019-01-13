#pragma once
#include "pch/pch.h"


class TestEnvironment : public ::testing::Environment
{
    public:

        virtual ~TestEnvironment() {}

        virtual void SetUp()
        {
            SetLoggerInstance(
                std::make_shared<spdlog::logger>(
                    "null_logger",
                    std::make_shared<spdlog::sinks::null_sink_mt>()
                )
            );
        }

        virtual void TearDown()
        {

        }
};
