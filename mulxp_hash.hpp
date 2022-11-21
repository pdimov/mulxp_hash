#ifndef MULXP_HASH_HPP_INCLUDED
#define MULXP_HASH_HPP_INCLUDED

// Copyright 2020-2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <cstdint>
#include <cstddef>
#include <cstring>

#if defined(_MSC_VER) && !defined(__clang__)

#include <intrin.h>

__forceinline std::uint64_t mulx( std::uint64_t x, std::uint64_t y )
{
    std::uint64_t r2;
    std::uint64_t r = _umul128( x, y, &r2 );
    return r ^ r2;
}

#else

inline std::uint64_t mulx( std::uint64_t x, std::uint64_t y )
{
    __uint128_t r = (__uint128_t)x * y;
    return (std::uint64_t)r ^ (std::uint64_t)( r >> 64 );
}

#endif

inline std::uint64_t read64le( unsigned char const * p )
{
    std::uint64_t r;
    std::memcpy( &r, p, 8 );
    return r;
}

inline std::uint32_t read32le( unsigned char const * p )
{
    std::uint32_t r;
    std::memcpy( &r, p, 4 );
    return r;
}

inline std::uint64_t mulxp0_hash( unsigned char const * p, std::size_t n, std::uint64_t seed )
{
    std::uint64_t const q = 0x9e3779b97f4a7c15ULL;
    std::uint64_t const k = q * q;

	std::uint64_t const n2 = n;

    std::uint64_t h = mulx( seed + q, k );

    while( n >= 8 )
    {
        std::uint64_t v1 = read64le( p );

        h ^= mulx( h + 1 + v1, k );

        p += 8;
        n -= 8;
    }

    {
        std::uint64_t v1 = 0;

        if( n >= 4 )
        {
            v1 = (std::uint64_t)read32le( p + n - 4 ) << ( n - 4 ) * 8 | read32le( p );
        }
        else if( n >= 1 )
        {
            std::size_t const x1 = ( n - 1 ) & 2; // 1: 0, 2: 0, 3: 2
            std::size_t const x2 = n >> 1;        // 1: 0, 2: 1, 3: 1

            v1 = (std::uint64_t)p[ x1 ] << x1 * 8 | (std::uint64_t)p[ x2 ] << x2 * 8 | (std::uint64_t)p[ 0 ];
        }

        h ^= mulx( h + 1 + v1, k );
    }

    return mulx( h + 1 + n2, k );
}

inline std::uint64_t mulxp1_hash( unsigned char const * p, std::size_t n, std::uint64_t seed )
{
    std::uint64_t const q = 0x9e3779b97f4a7c15ULL;
    std::uint64_t const k = q * q;

    std::uint64_t w = mulx( seed + q, k );
    std::uint64_t h = w ^ n;

    while( n >= 8 )
    {
        std::uint64_t v1 = read64le( p );

        w += q;
        h ^= mulx( v1 + w, k );

        p += 8;
        n -= 8;
    }

    {
        std::uint64_t v1 = 0;

        if( n >= 4 )
        {
            v1 = (std::uint64_t)read32le( p + n - 4 ) << ( n - 4 ) * 8 | read32le( p );
        }
        else if( n >= 1 )
        {
            std::size_t const x1 = ( n - 1 ) & 2; // 1: 0, 2: 0, 3: 2
            std::size_t const x2 = n >> 1;        // 1: 0, 2: 1, 3: 1

            v1 = (std::uint64_t)p[ x1 ] << x1 * 8 | (std::uint64_t)p[ x2 ] << x2 * 8 | (std::uint64_t)p[ 0 ];
        }

        w += q;
        h ^= mulx( v1 + w, k );
    }

    return mulx( h + w, k );
}

inline std::uint64_t mulxp2_hash( unsigned char const * p, std::size_t n, std::uint64_t seed )
{
    std::uint64_t const q = 0x9e3779b97f4a7c15ULL;
    std::uint64_t const k = q * q;

	std::uint64_t const n2 = n;

    std::uint64_t w = mulx( seed + q, k );
    std::uint64_t h = w;

    while( n >= 16 )
    {
        std::uint64_t v1 = read64le( p + 0 );
        std::uint64_t v2 = read64le( p + 8 );

        w += q;
        h ^= mulx( v1 + w, k );

        w += q;
        h ^= mulx( v2 + w, k );

        p += 16;
        n -= 16;
    }

    {
        std::uint64_t v1 = 0;
        std::uint64_t v2 = 0;

        if( n > 8 )
        {
            v1 = read64le( p );
            v2 = read64le( p + n - 8 ) >> ( 16 - n ) * 8;
        }
        else if( n >= 4 )
        {
            v1 = (std::uint64_t)read32le( p + n - 4 ) << ( n - 4 ) * 8 | read32le( p );
        }
        else if( n >= 1 )
        {
            std::size_t const x1 = ( n - 1 ) & 2; // 1: 0, 2: 0, 3: 2
            std::size_t const x2 = n >> 1;        // 1: 0, 2: 1, 3: 1

            v1 = (std::uint64_t)p[ x1 ] << x1 * 8 | (std::uint64_t)p[ x2 ] << x2 * 8 | (std::uint64_t)p[ 0 ];
        }

        w += q;
        h ^= mulx( v1 + w, k );

        w += q;
        h ^= mulx( v2 + w, k );
    }

    return mulx( h + w, k + n2 );
}

inline std::uint64_t mulxp3_hash( unsigned char const * p, std::size_t n, std::uint64_t seed )
{
    std::uint64_t const q = 0x9e3779b97f4a7c15ULL;
    std::uint64_t const k = q * q;

    std::uint64_t w = mulx( seed + q, k );
    std::uint64_t h = w ^ n;

    while( n >= 16 )
    {
        std::uint64_t v1 = read64le( p + 0 );
        std::uint64_t v2 = read64le( p + 8 );

        w += q;
        h ^= mulx( v1 + w, v2 + w + k );

        p += 16;
        n -= 16;
    }

    {
        std::uint64_t v1 = 0;
        std::uint64_t v2 = 0;

        if( n > 8 )
        {
            v1 = read64le( p );
            v2 = read64le( p + n - 8 ) >> ( 16 - n ) * 8;
        }
        else if( n >= 4 )
        {
            v1 = (std::uint64_t)read32le( p + n - 4 ) << ( n - 4 ) * 8 | read32le( p );
        }
        else if( n >= 1 )
        {
            std::size_t const x1 = ( n - 1 ) & 2; // 1: 0, 2: 0, 3: 2
            std::size_t const x2 = n >> 1;        // 1: 0, 2: 1, 3: 1

            v1 = (std::uint64_t)p[ x1 ] << x1 * 8 | (std::uint64_t)p[ x2 ] << x2 * 8 | (std::uint64_t)p[ 0 ];
        }

        w += q;
        h ^= mulx( v1 + w, v2 + w + k );
    }

    return mulx( h, k );
}

inline void mulxp0_test( void const * blob, int len, std::uint32_t seed, void * out )
{
    std::uint64_t h = mulxp0_hash( static_cast<unsigned char const *>( blob ), len, seed );
    *static_cast<std::uint64_t*>( out ) = h;
}

inline void mulxp1_test( void const * blob, int len, std::uint32_t seed, void * out )
{
    std::uint64_t h = mulxp1_hash( static_cast<unsigned char const *>( blob ), len, seed );
    *static_cast<std::uint64_t*>( out ) = h;
}

inline void mulxp2_test( void const * blob, int len, std::uint32_t seed, void * out )
{
    std::uint64_t h = mulxp2_hash( static_cast<unsigned char const *>( blob ), len, seed );
    *static_cast<std::uint64_t*>( out ) = h;
}

inline void mulxp3_test( void const * blob, int len, std::uint32_t seed, void * out )
{
    std::uint64_t h = mulxp3_hash( static_cast<unsigned char const *>( blob ), len, seed );
    *static_cast<std::uint64_t*>( out ) = h;
}

#endif // #ifndef MULXP_HASH_HPP_INCLUDED
