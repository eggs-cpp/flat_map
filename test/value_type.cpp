// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("value_type/construct", "[value_type]")
{
    value_type<int, float, std::string> v(1, 2.f, "hello");

    CHECK(v.first == 1);
    CHECK(v.value<0>() == 2.f);
    CHECK(v.value<1>() == "hello");
}

TEST_CASE("value_type/construct/perfect_forward", "[value_type]")
{
    std::string s = "hello";
    value_type<int, float, std::string> v(1, 2.f, std::move(s));

    CHECK(v.value<1>() == "hello");
    CHECK(s.empty());
}

TEST_CASE("value_type/value/mutable", "[value_type]")
{
    value_type<int, float, std::string> v(1, 2.f, "x");

    v.value<0>() = 3.f;
    v.value<1>() = "y";

    CHECK(v.value<0>() == 3.f);
    CHECK(v.value<1>() == "y");
}

TEST_CASE("value_type/value/const", "[value_type]")
{
    const value_type<int, float, std::string> v(1, 2.f, "x");

    CHECK(v.value<0>() == 2.f);
    CHECK(v.value<1>() == "x");
}

TEST_CASE("value_type/first/const", "[value_type]")
{
    value_type<int, float> v(1, 1.f);

    // first is const — must not compile if uncommented:
    // v.first = 2;
    CHECK(v.first == 1);
}

TEST_CASE("value_type/structured_bindings", "[value_type]")
{
    value_type<int, float, std::string> v(1, 2.f, "hi");

    auto [k, f, s] = v;

    CHECK(k == 1);
    CHECK(f == 2.f);
    CHECK(s == "hi");
}

TEST_CASE("value_type/structured_bindings/zero_values", "[value_type]")
{
    // degenerate case: Key() — no values
    value_type<int> v(42);

    auto [k] = v;

    CHECK(k == 42);
}
