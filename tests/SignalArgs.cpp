//
// Created by samuel on 02/11/2021.
//

#include "signal.hpp"
#include <gtest/gtest.h>
#include <iostream>

TEST(Signal, OneParameter)
{
    int result = 0;
    fox::signal<int> int_sig;

    int_sig.connect([&](int v) {
        result = v;
    });

    EXPECT_TRUE(result == 0);

    // emit a signal
    int_sig.emit(2);
    EXPECT_TRUE(result == 2);

    short i = 4; // Convertible to int
    int_sig.emit(i);
    EXPECT_TRUE(result == i);
}

TEST(Signal, MultipleParameters)
{
    int iResult = 0;
    float fResult = 0;

    fox::signal<float, int, bool> sig;

    // Notice how we accept a double as first argument here.
    // This is fine because float is convertible to double.
    sig.connect([&](double f, int i, bool b) {
        if (b) {
            iResult = i;
            fResult = f;
        }
    });

    EXPECT_TRUE(iResult == 0);
    EXPECT_TRUE(fResult == 0.f);

    // emit a signal
    sig.emit(2.f, 5, false);

    EXPECT_TRUE(iResult == 0);
    EXPECT_TRUE(fResult == 0.f);

    sig.emit(4.5f, 4, true);

    EXPECT_TRUE(iResult == 4);
    EXPECT_TRUE(fResult == 4.5f);
}

TEST(Signal, ReferenceParameter)
{
    int iResult = 0;
    fox::signal<int&, int> sig;

    sig.connect([](int& i, int val) {
        i = val;
    });

    EXPECT_TRUE(iResult == 0);

    // emit a signal
    sig.emit(iResult, 5);
    EXPECT_TRUE(iResult == 5);
    sig.emit(iResult, 1);
    EXPECT_TRUE(iResult == 1);
}
