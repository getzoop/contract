
#ifndef BOOST_CONTRACT_EXCEPTION_HPP_
#define BOOST_CONTRACT_EXCEPTION_HPP_

#include <boost/config.hpp>
#include <exception>
#include <string>
#include <sstream>
#include <iostream>

namespace boost { namespace contract {

// Exceptions.

struct assertion_failure : public std::exception {
    explicit assertion_failure(char const* const file = "",
            unsigned long const line = 0, char const* const code = "") :
        file_(file), line_(line), code_(code)
    { init(); }
    
    explicit assertion_failure(char const* const code) :
            file_(""), line_(0), code_(code)
    { init(); }

    virtual ~assertion_failure() {}

    // Return `assertion "XYZ" failed: file "ABC", line 123`.
    virtual char const* what() const BOOST_NOEXCEPT { return what_.c_str(); }

    char const* const file() const { return file_; }
    unsigned long line() const { return line_; }
    char const* const code() const { return code_; }

private:
    void init() {
        std::ostringstream text;
        text << "assertion";
        if(std::string(code_) != "") text << " \"" << code_ << "\"";
        text << " failed";
        if(std::string(file_) != "") {
            text << ": \"" << file_ << "\"";
            if(line_ != 0) text << ", line " << line_;
        }
        what_ = text.str();
    }

    char const* const file_;
    unsigned long const line_;
    char const* const code_;
    std::string what_;
};

// Handlers.

enum from {
    from_constructor,
    from_destructor,
    from_public_member,
    from_protected_member,
    from_private_member,
    from_free_function
};

typedef void (*failure_handler)(from);

// TODO: These are part of the lib state. They should prob be put in a .cpp and
// exported (so they are the same across DLL, etc.), plus they should be
// protected by mutexes.
namespace aux {
    namespace exception_ {
        enum failure_handler_key {
            pre_key,
            post_key,
            const_entry_inv_key,
            const_volatile_entry_inv_key,
            static_entry_inv_key,
            const_exit_inv_key,
            const_volatile_exit_inv_key,
            static_exit_inv_key,
        };

        template<exception_::failure_handler_key Key>
        void default_failure_handler(from const) {
            std::string s = "";
            switch(Key) {
                case pre_key:
                    s = "precondition "; break;
                case post_key:
                    s = "postcondition "; break;
                case const_entry_inv_key:
                    s = "const entry invariant "; break;
                case const_volatile_entry_inv_key:
                    s = "const volatile entry invariant "; break;
                case static_entry_inv_key:
                    s = "static entry invariant "; break;
                case const_exit_inv_key:
                    s= "const exit invariant "; break;
                case const_volatile_exit_inv_key:
                    s= "const volatile exit invariant "; break;
                case static_exit_inv_key:
                    s = "static exit invariant "; break;
                // No default (so compiler warning/error on missing enum case).
            }
            try {
                throw;
            } catch(boost::contract::assertion_failure const& error) {
                // what = 'assertion "..." failed: ...'.
                std::cerr << s << error.what() << std::endl;
            } catch(std::exception const& error) {
                std::cerr << s << "checking threw standard exception with " <<
                        "what(): " << error.what() << std::endl;
            } catch(...) {
                std::cerr << s << "checking threw unknown exception" <<
                        std::endl;
            }
            std::terminate(); // Default handlers log and call terminate.
        }
    }

    failure_handler pre_failure_handler = &exception_::
            default_failure_handler<exception_::pre_key>;
    failure_handler post_failure_handler = &exception_::
            default_failure_handler<exception_::post_key>;
    
    failure_handler const_entry_inv_failure_handler = &exception_::
            default_failure_handler<exception_::const_entry_inv_key>;
    failure_handler const_volatile_entry_inv_failure_handler = &exception_::
            default_failure_handler<exception_::const_volatile_entry_inv_key>;
    failure_handler static_entry_inv_failure_handler = &exception_::
            default_failure_handler<exception_::static_entry_inv_key>;
    
    failure_handler const_exit_inv_failure_handler = &exception_::
            default_failure_handler<exception_::const_exit_inv_key>;
    failure_handler const_volatile_exit_inv_failure_handler = &exception_::
            default_failure_handler<exception_::const_volatile_exit_inv_key>;
    failure_handler static_exit_inv_failure_handler = &exception_::
            default_failure_handler<exception_::static_exit_inv_key>;
}

failure_handler set_precondition_failure(failure_handler f)
        BOOST_NOEXCEPT_OR_NOTHROW {
    failure_handler result = boost::contract::aux::pre_failure_handler;
    boost::contract::aux::pre_failure_handler = f;
    return result;
}

failure_handler set_postcondition_failure(failure_handler f)
        BOOST_NOEXCEPT_OR_NOTHROW {
    failure_handler result = boost::contract::aux::post_failure_handler;
    boost::contract::aux::post_failure_handler = f;
    return result;
}

// Entry invariants.

failure_handler set_const_entry_invariant_failure(failure_handler f)
        BOOST_NOEXCEPT_OR_NOTHROW {
    failure_handler result = boost::contract::aux::
            const_entry_inv_failure_handler;
    boost::contract::aux::const_entry_inv_failure_handler = f;
    return result;
}

failure_handler set_const_volatile_entry_invariant_failure(failure_handler f)
        BOOST_NOEXCEPT_OR_NOTHROW {
    failure_handler result = boost::contract::aux::
            const_volatile_entry_inv_failure_handler;
    boost::contract::aux::const_volatile_entry_inv_failure_handler = f;
    return result;
}

failure_handler set_static_entry_invariant_failure(failure_handler f)
        BOOST_NOEXCEPT_OR_NOTHROW {
    failure_handler result = boost::contract::aux::
            static_entry_inv_failure_handler;
    boost::contract::aux::static_entry_inv_failure_handler = f;
    return result;
}

void set_entry_invariant_failure(failure_handler f) BOOST_NOEXCEPT_OR_NOTHROW {
    set_const_entry_invariant_failure(f);
    set_const_volatile_entry_invariant_failure(f);
    set_static_entry_invariant_failure(f);
}

// Exit invariants.

failure_handler set_const_exit_invariant_failure(failure_handler f)
        BOOST_NOEXCEPT_OR_NOTHROW {
    failure_handler result = boost::contract::aux::
            const_exit_inv_failure_handler;
    boost::contract::aux::const_exit_inv_failure_handler = f;
    return result;
}

failure_handler set_const_volatile_exit_invariant_failure(failure_handler f)
        BOOST_NOEXCEPT_OR_NOTHROW {
    failure_handler result = boost::contract::aux::
            const_volatile_exit_inv_failure_handler;
    boost::contract::aux::const_volatile_exit_inv_failure_handler = f;
    return result;
}

failure_handler set_static_exit_invariant_failure(failure_handler f)
        BOOST_NOEXCEPT_OR_NOTHROW {
    failure_handler result = boost::contract::aux::
            static_exit_inv_failure_handler;
    boost::contract::aux::static_exit_inv_failure_handler = f;
    return result;
}

void set_exit_invariant_failure(failure_handler f) BOOST_NOEXCEPT_OR_NOTHROW {
    set_const_exit_invariant_failure(f);
    set_const_volatile_exit_invariant_failure(f);
    set_static_exit_invariant_failure(f);
}

// All invariants.

void set_invariant_failure(failure_handler f) BOOST_NOEXCEPT_OR_NOTHROW {
    set_entry_invariant_failure(f);
    set_exit_invariant_failure(f);
}

} } // namespace

#endif // #include guard

