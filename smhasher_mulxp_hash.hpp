#ifndef SMHASHER_MULXP_HASH_HPP_INCLUDED
#define SMHASHER_MULXP_HASH_HPP_INCLUDED

// Copyright 2020-2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "mulxp_hash.hpp"
#include <cstdint>

inline void smhasher_mulxp0_test( void const * blob, int len, std::uint32_t seed, void * out )
{
    std::uint64_t h = mulxp0_hash( static_cast<unsigned char const *>( blob ), len, seed );
    *static_cast<std::uint64_t*>( out ) = h;
}

inline void smhasher_mulxp1_test( void const * blob, int len, std::uint32_t seed, void * out )
{
    std::uint64_t h = mulxp1_hash( static_cast<unsigned char const *>( blob ), len, seed );
    *static_cast<std::uint64_t*>( out ) = h;
}

inline void smhasher_mulxp2_test( void const * blob, int len, std::uint32_t seed, void * out )
{
    std::uint64_t h = mulxp2_hash( static_cast<unsigned char const *>( blob ), len, seed );
    *static_cast<std::uint64_t*>( out ) = h;
}

inline void smhasher_mulxp3_test( void const * blob, int len, std::uint32_t seed, void * out )
{
    std::uint64_t h = mulxp3_hash( static_cast<unsigned char const *>( blob ), len, seed );
    *static_cast<std::uint64_t*>( out ) = h;
}

//

inline void smhasher_mulxp3_test32( void const * blob, int len, std::uint32_t seed, void * out )
{
    std::uint32_t h = mulxp3_hash32( static_cast<unsigned char const *>( blob ), len, seed );
    *static_cast<std::uint32_t*>( out ) = h;
}

#endif // #ifndef SMHASHER_MULXP_HASH_HPP_INCLUDED
