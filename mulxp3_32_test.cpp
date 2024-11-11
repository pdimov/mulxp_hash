// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "mulxp_hash.hpp"
#include <vector>
#include <cstdio>

// Test vectors for mulxp1_hash32

struct reference_entry
{
    std::size_t length;
    std::uint32_t seed;
    std::uint32_t result;
};

reference_entry const ref[] =
{
    {   0, 0x00000000, 0x8E655EAE },
    {   0, 0x9E3779B9, 0x0283AE69 },
    {  17, 0x00000000, 0x69931AE7 },
    {  17, 0x9E3779B9, 0x946D68AF },
    {  34, 0x00000000, 0xEBFAD8A5 },
    {  34, 0x9E3779B9, 0x7C8FE598 },
    {  51, 0x00000000, 0xAA337B35 },
    {  51, 0x9E3779B9, 0x2E027F82 },
    {  68, 0x00000000, 0x42BABC15 },
    {  68, 0x9E3779B9, 0xDFFFB879 },
    {  85, 0x00000000, 0x1A39F5C2 },
    {  85, 0x9E3779B9, 0xCA5D002A },
    { 102, 0x00000000, 0xCA101728 },
    { 102, 0x9E3779B9, 0x7E135B64 },
    { 119, 0x00000000, 0xD95677AE },
    { 119, 0x9E3779B9, 0x9230B6D4 },
    { 136, 0x00000000, 0xD9E48D12 },
    { 136, 0x9E3779B9, 0xFB27B42D },
    { 153, 0x00000000, 0x2C6040EB },
    { 153, 0x9E3779B9, 0xB1DFD3A4 },
    { 170, 0x00000000, 0x6B8F0E37 },
    { 170, 0x9E3779B9, 0x420E68A8 },
    { 187, 0x00000000, 0x6EA8D6AC },
    { 187, 0x9E3779B9, 0x12729B87 },
    { 204, 0x00000000, 0x11568A19 },
    { 204, 0x9E3779B9, 0x453D7330 },
    { 221, 0x00000000, 0x733B857D },
    { 221, 0x9E3779B9, 0xAB8AED59 },
    { 238, 0x00000000, 0x9EF34228 },
    { 238, 0x9E3779B9, 0xA0C348EC },
    { 255, 0x00000000, 0x5ACF71B8 },
    { 255, 0x9E3779B9, 0xFB410806 },
    { 272, 0x00000000, 0xFF645816 },
    { 272, 0x9E3779B9, 0xF1EBACDD },
};

int main()
{
    int errors = 0;

    for( int i = 0; i < sizeof(ref) / sizeof(ref[0]); ++i )
    {
        reference_entry const& e = ref[ i ];

        std::vector<unsigned char> v( e.length, 0x9C );

        std::uint32_t r = mulxp3_hash32( v.data(), v.size(), e.seed );

        if( r != e.result )
        {
            std::fprintf( stderr, "Reference value mismatch for length %zu, seed %08X: was %08X, expected %08X\n", e.length, e.seed, r, e.result );
            ++errors;
        }
    }

    if( errors == 0 )
    {
        std::puts( "No errors detected." );
    }

    return errors;
}
