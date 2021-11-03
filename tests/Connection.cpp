//
// Created by samuel on 02/11/2021.
//

#include "signal.hpp"
#include <gtest/gtest.h>
#include <iostream>

TEST(Connection, SimpleDeconnection)
{
    fox::signal<> void_sig;
    int i = 0;

    auto conn = void_sig.connect([&](){
        i++;
    });
    void_sig.emit();

    EXPECT_TRUE(i == 1);
    EXPECT_TRUE(conn.is_connected());
    conn.disconnect();
    void_sig.emit();
    EXPECT_TRUE(i == 1);
    EXPECT_FALSE(conn.is_connected());
}
