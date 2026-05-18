// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("iterators/begin/end", "[iterators]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    CHECK(std::distance(m.begin(), m.end()) == 2);
}

TEST_CASE("iterators/cbegin/cend", "[iterators]")
{
    flat_map<int(float)> m{{1, 1.f}};

    CHECK(std::distance(m.cbegin(), m.cend()) == 1);
}

TEST_CASE("iterators/rbegin/rend", "[iterators]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    CHECK(m.rbegin()->first == 2);
    CHECK(std::distance(m.rbegin(), m.rend()) == 2);
}

TEST_CASE("iterators/deref/first", "[iterators]")
{
    flat_map<int(float)> m{{1, 1.f}};

    CHECK(m.begin()->first == 1);
}

TEST_CASE("iterators/deref/value", "[iterators]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    CHECK(m.begin()->value<0>() == 1.f);
    CHECK(m.begin()->value<1>() == "a");
}

TEST_CASE("iterators/deref/structured_bindings", "[iterators]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    auto ref = *m.begin();
    auto& [k, f, s] = ref;

    CHECK(k == 1);
    CHECK(f == 1.f);
    CHECK(s == "a");
}

TEST_CASE("iterators/deref/assign", "[iterators]")
{
    flat_map<int(float)> m{{1, 1.f}};

    *m.begin() = {1, 2.f};

    CHECK(m.begin()->value<0>() == 2.f);
}

TEST_CASE("iterators/sorted_order", "[iterators]")
{
    flat_map<int(float)> m{{3, 3.f}, {1, 1.f}, {2, 2.f}};

    auto it = m.begin();
    CHECK(it->first == 1);
    ++it;
    CHECK(it->first == 2);
    ++it;
    CHECK(it->first == 3);
}

TEST_CASE("iterators/arithmetic/advance", "[iterators]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}, {3, 3.f}};

    auto it = m.begin() + 2;

    CHECK(it->first == 3);
}

TEST_CASE("iterators/arithmetic/distance", "[iterators]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}, {3, 3.f}};

    CHECK(m.end() - m.begin() == 3);
}

TEST_CASE("iterators/comparison", "[iterators]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    CHECK(m.begin() < m.end());
    CHECK(m.begin() != m.end());
    CHECK(m.begin() == m.begin());
}

TEST_CASE("iterators/const/from_mutable", "[iterators]")
{
    flat_map<int(float)> m{{1, 1.f}};

    flat_map<int(float)>::const_iterator it = m.begin();

    CHECK(it->first == 1);
    CHECK(it->value<0>() == 1.f);
}
