
// Copyright (C) 2008-2012 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt)
// Home at http://sourceforge.net/projects/contractpp

#ifndef CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_HPP_
#define CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_HPP_

#include <contract/detail/preprocessor/keyword/facilities/is.hpp>
#include <contract/detail/preprocessor/keyword/facilities/add.hpp>
#include <contract/detail/preprocessor/keyword/facilities/remove.hpp>

// PRIVATE //

// These are not local macros -- DO NOT #UNDEF.
#define CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_IS_namespace (1) /* unary */
#define namespace_CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_IS (1) /* unary */
#define CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_REMOVE_namespace /* nothing */
#define namespace_CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_REMOVE /* nothing */

// PUBLIC //

// Is.

#define CONTRACT_DETAIL_PP_KEYWORD_IS_NAMESPACE_FRONT(tokens) \
    CONTRACT_DETAIL_PP_KEYWORD_FACILITY_IS_FRONT(tokens, \
            CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_IS_)

#define CONTRACT_DETAIL_PP_KEYWORD_IS_NAMESPACE_BACK(token) \
    CONTRACT_DETAIL_PP_KEYWORD_FACILITY_IS_BACK(token, \
            _CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_IS)

// Remove.

#define CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_REMOVE_FRONT(tokens) \
    CONTRACT_DETAIL_PP_KEYWORD_FACILITY_REMOVE_FRONT(tokens, \
            CONTRACT_DETAIL_PP_KEYWORD_IS_NAMESPACE_FRONT, \
            CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_REMOVE_)

#define CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_REMOVE_BACK(tokens) \
    CONTRACT_DETAIL_PP_KEYWORD_FACILITY_REMOVE_BACK(tokens, \
            CONTRACT_DETAIL_PP_KEYWORD_IS_NAMESPACE_BACK, \
            _CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_REMOVE)

// Add.

#define CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_ADD_FRONT(tokens) \
    CONTRACT_DETAIL_PP_KEYWORD_FACILITY_ADD_FRONT(tokens, \
            CONTRACT_DETAIL_PP_KEYWORD_IS_NAMESPACE_FRONT, namespace)

#define CONTRACT_DETAIL_PP_KEYWORD_NAMESPACE_ADD_BACK(tokens) \
    CONTRACT_DETAIL_PP_KEYWORD_FACILITY_ADD_BACK(tokens, \
            CONTRACT_DETAIL_PP_KEYWORD_IS_NAMESPACE_BACK, namespace)

#endif // #include guard

