#ifndef BOOST_HASH_HPP_INCLUDED
#define BOOST_HASH_HPP_INCLUDED

// Copyright 2020-2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/container_hash/hash.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>

inline void boost_test( void const * blob, int len, std::uint32_t seed_, void * out )
{
	unsigned char const* p = static_cast<unsigned char const*>( blob );

	std::size_t seed = seed_;

	boost::hash_range( seed, p, p + len );

	*static_cast<std::uint64_t*>( out ) = seed;
}

#endif // #ifndef BOOST_HASH_HPP_INCLUDED
