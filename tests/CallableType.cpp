//
// Created by samuel on 02/11/2021.
//

#include "signal.hpp"
#include <gtest/gtest.h>
#include <iostream>

void normal_func() { std::cout << "free function\n"; }

struct structure
{
    void member_func() { std::cout << "member function\n"; }
    void const_member_func() const { std::cout << "const member function\n"; }
    static void static_member_func() { std::cout << "static member function\n";  }
};

struct op
{
    void operator()() { std::cout << "function object\n"; }
};

TEST(Callable, NormalFunction)
{
    fox::signal<> void_sig;
    std::string test_string = "free function\n";

    testing::internal::CaptureStdout();
    void_sig.connect(normal_func);
    void_sig.emit();
    EXPECT_TRUE(testing::internal::GetCapturedStdout() == test_string);
}

TEST(Callable, LambdaFunction)
{
    auto lambda = []() { std::cout << "lambda\n"; };

    fox::signal<> void_sig;
    std::string test_string = "lambda\n";

    testing::internal::CaptureStdout();
    void_sig.connect(lambda);
    void_sig.emit();
    EXPECT_TRUE(testing::internal::GetCapturedStdout() == test_string);
}

TEST(Callable, GenericLambdaFunction)
{
    auto gen_lambda = [](auto && ...a) { std::cout << "generic lambda\n"; };

    fox::signal<> void_sig;
    std::string test_string = "generic lambda\n";

    testing::internal::CaptureStdout();
    void_sig.connect(gen_lambda);
    void_sig.emit();
    EXPECT_TRUE(testing::internal::GetCapturedStdout() == test_string);
}

TEST(Callable, ClassFunction)
{
    structure my_struct;
    fox::signal<> void_sig;
    std::string test_string = "member function\n";

    testing::internal::CaptureStdout();
    void_sig.connect(&my_struct, &structure::member_func);
    void_sig.emit();
    EXPECT_TRUE(testing::internal::GetCapturedStdout() == test_string);
}

TEST(Callable, ConstClassFunction)
{
    structure my_struct;
    fox::signal<> void_sig;
    std::string test_string = "const member function\n";

    testing::internal::CaptureStdout();
    void_sig.connect(&my_struct, &structure::const_member_func);
    void_sig.emit();
    EXPECT_TRUE(testing::internal::GetCapturedStdout() == test_string);
}

TEST(Callable, StaticClassFunction)
{
    fox::signal<> void_sig;
    std::string test_string = "static member function\n";

    testing::internal::CaptureStdout();
    void_sig.connect(&structure::static_member_func);
    void_sig.emit();
    EXPECT_TRUE(testing::internal::GetCapturedStdout() == test_string);
}

TEST(Callable, ClassOperatorFunction)
{
    fox::signal<> void_sig;
    std::string test_string = "function object\n";

    testing::internal::CaptureStdout();
    void_sig.connect(op());
    void_sig.emit();
    EXPECT_TRUE(testing::internal::GetCapturedStdout() == test_string);
}
