
// Copyright (C) 2008-2017 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0 (see accompanying
// file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).
// See: http://www.boost.org/doc/libs/release/libs/contract/doc/html/index.html

// Test post after except error (same if not free func).

#include <boost/contract/function.hpp>
#include <boost/contract/check.hpp>

void f() {
    boost::contract::check c = boost::contract::function()
        .except([] {})
        .postcondition([] {}) // Error (post after except).
    ;
}

int main() {
    f();
    return 0;
}

