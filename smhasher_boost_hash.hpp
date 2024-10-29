#ifndef SMHASHER_BOOST_HASH_HPP_INCLUDED
#define SMHASHER_BOOST_HASH_HPP_INCLUDED

// Copyright 2020-2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/container_hash/hash.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>

inline void smhasher_boost_test( void const * blob, int len, std::uint32_t seed_, void * out )
{
    unsigned char const* p = static_cast<unsigned char const*>( blob );

    std::size_t seed = seed_;

    boost::hash_range( seed, p, p + len );

    *static_cast<std::uint64_t*>( out ) = seed;
}

// 32 bit

inline void smhasher_hash_combine_32( std::uint32_t& seed, std::uint32_t v )
{
    seed = boost::hash_detail::hash_mix_impl<32>::fn( seed + 0x9e3779b9 + v );
}

inline std::uint32_t smhasher_hash_range_32( std::uint32_t seed, unsigned char const* first, unsigned char const* last )
{
    std::size_t n = static_cast<std::size_t>( last - first );

    for( ; n >= 4; first += 4, n -= 4 )
    {
        // clang 5+, gcc 5+ figure out this pattern and use a single mov on x86
        // gcc on s390x and power BE even knows how to use load-reverse

        std::uint32_t w =
            static_cast<std::uint32_t>( static_cast<unsigned char>( first[0] ) ) |
            static_cast<std::uint32_t>( static_cast<unsigned char>( first[1] ) ) <<  8 |
            static_cast<std::uint32_t>( static_cast<unsigned char>( first[2] ) ) << 16 |
            static_cast<std::uint32_t>( static_cast<unsigned char>( first[3] ) ) << 24;

        hash_combine( seed, w );
    }

    {
        // add a trailing suffix byte of 0x01 because otherwise sequences of
        // trailing zeroes are indistinguishable from end of string

        std::uint32_t w = 0x01u;

        switch( n )
        {
        case 1:

            w =
                static_cast<std::uint32_t>( static_cast<unsigned char>( first[0] ) ) |
                0x0100u;

            break;

        case 2:

            w =
                static_cast<std::uint32_t>( static_cast<unsigned char>( first[0] ) ) |
                static_cast<std::uint32_t>( static_cast<unsigned char>( first[1] ) ) <<  8 |
                0x010000u;

            break;

        case 3:

            w =
                static_cast<std::uint32_t>( static_cast<unsigned char>( first[0] ) ) |
                static_cast<std::uint32_t>( static_cast<unsigned char>( first[1] ) ) <<  8 |
                static_cast<std::uint32_t>( static_cast<unsigned char>( first[2] ) ) << 16 |
                0x01000000u;

            break;
        }

        hash_combine( seed, w );
    }

    return seed;
}

inline void smhasher_boost_test_32( void const * blob, int len, std::uint32_t seed, void * out )
{
    unsigned char const* p = static_cast<unsigned char const*>( blob );

    seed = smhasher_hash_range_32( seed, p, p + len );

    *static_cast<std::uint32_t*>( out ) = seed;
}

#endif // #ifndef SMHASHER_BOOST_HASH_HPP_INCLUDED
