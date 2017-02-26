
// Copyright (C) 2008-2016 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0 (see accompanying
// file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).
// See: http://www.boost.org/doc/libs/release/libs/contract/doc/html/index.html

#include <boost/contract.hpp>
#include <iostream>
#include <cstring>
#include <cassert>

//[throw_on_failure_cstring
struct too_large_error {};

template<unsigned MaxSize>
class cstring
    #define BASES private boost::contract::constructor_precondition<cstring< \
            MaxSize> >
    : BASES
{
public:
    typedef BOOST_CONTRACT_BASE_TYPES(BASES) base_types;
    #undef BASES

    /* implicit */ cstring(char const* chars) :
        boost::contract::constructor_precondition<cstring>([&] {
            BOOST_CONTRACT_ASSERT(chars); // Throw `assertion_failure`.
            // Or, throw user-defined exception.
            if(std::strlen(chars) > MaxSize) throw too_large_error();
        })
    {

        /* ... */
//]
        boost::contract::check c = boost::contract::constructor(this)
            .postcondition([&] {
                BOOST_CONTRACT_ASSERT(size() == std::strlen(chars));
            })
        ;

        size_ = std::strlen(chars);
        for(unsigned i = 0; i < size_; ++i) chars_[i] = chars[i];
        chars_[size_] = '\0';
    }

    ~cstring() {
        // Check invariants.
        boost::contract::check c = boost::contract::destructor(this);
    }

    unsigned size() const {
        // Check invariants.
        boost::contract::check c = boost::contract::public_function(this);
        return size_;
    }
    
    void invariant() const {
        if(size() > MaxSize) throw too_large_error(); // Throw user-defined ex.
        BOOST_CONTRACT_ASSERT(chars_); // Or, throw `assertion_failure`.
        BOOST_CONTRACT_ASSERT(chars_[size()] == '\0');
    }

private:
    char chars_[MaxSize + 1];
    unsigned size_;
};

//[throw_on_failure_handler
int main() {
    boost::contract::set_precondition_failure(
    boost::contract::set_postcondition_failure(
    boost::contract::set_invariant_failure(
    boost::contract::set_old_failure(
        [] (boost::contract::from where) {
            if(where == boost::contract::from_destructor) {
                // Cannot throw from destructors in C++.
                std::clog << "ignored destructor contract failure" << std::endl;
            } else throw; // Re-throw (assertion_failure, user-defined, etc.).
        }
    ))));
    boost::contract::set_except_failure(
        [] (boost::contract::from where) {
            // Already an active exception so can't throw another in C++.
            std::clog << "ignored exception guarantee failure" << std::endl;
        }
    );
    boost::contract::set_check_failure(
        [] {
            // But now CHECK cannot be used within destructor implementations.
            throw; // Re-throw (assertion_failure, user-defined, etc.).
        }
    );

    /* ... */
//]

    {
        cstring<3> s("abc");
        assert(s.size() == 3);
    }

    #ifndef BOOST_CONTRACT_NO_PRECONDITIONS
        // These failures properly handled only when preconditions checked.

        try {
            char* c = 0;
            cstring<3> s(c);
            assert(false);
        } catch(boost::contract::assertion_failure const& error) {
            // OK (expected).
            std::clog << "ignored: " << error.what() << std::endl;
        } catch(...) { assert(false); }
        
        try {
            cstring<3> s("abcd");
            assert(false);
        } catch(too_large_error const&) {} // OK (expected).
        catch(...) { assert(false); }
    #endif

    return 0;
}

