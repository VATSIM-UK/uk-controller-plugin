#pragma once

class TestEnvironment : public ::testing::Environment
{
    public:
    static auto CreateEnvironment() -> TestEnvironment*;

    virtual void SetUp();
    virtual void TearDown();

    private:
    ULONG_PTR gdiPlusToken;
};
