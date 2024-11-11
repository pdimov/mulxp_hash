// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "mulxp_hash.hpp"
#include <vector>
#include <cstdio>
#include <cinttypes>

// Test vectors for mulxp1_hash32

struct reference_entry
{
    std::size_t length;
    std::uint32_t seed;
    std::uint32_t result;
};

reference_entry const ref[] =
{
    {   0, 0x00000000, 0x6F5D607C },
    {   0, 0x9E3779B9, 0x49DAC063 },
    {  17, 0x00000000, 0xA9830593 },
    {  17, 0x9E3779B9, 0xA3ED9324 },
    {  34, 0x00000000, 0x580A9DFB },
    {  34, 0x9E3779B9, 0x79334B61 },
    {  51, 0x00000000, 0xEC81A87C },
    {  51, 0x9E3779B9, 0x089B2005 },
    {  68, 0x00000000, 0x044EB1FA },
    {  68, 0x9E3779B9, 0xA9E373FA },
    {  85, 0x00000000, 0xBD925846 },
    {  85, 0x9E3779B9, 0x9C636C64 },
    { 102, 0x00000000, 0x1D6857EA },
    { 102, 0x9E3779B9, 0x57866FC5 },
    { 119, 0x00000000, 0xB1F4C4A5 },
    { 119, 0x9E3779B9, 0x70C6F1E3 },
    { 136, 0x00000000, 0x2F519887 },
    { 136, 0x9E3779B9, 0x26E25BC8 },
    { 153, 0x00000000, 0x4923B36C },
    { 153, 0x9E3779B9, 0x2E25C187 },
    { 170, 0x00000000, 0xB9FF9CB5 },
    { 170, 0x9E3779B9, 0xC0A36E75 },
    { 187, 0x00000000, 0x9DC48EA3 },
    { 187, 0x9E3779B9, 0x29C12DF6 },
    { 204, 0x00000000, 0x42495C84 },
    { 204, 0x9E3779B9, 0x4A32B5E1 },
    { 221, 0x00000000, 0x1A1E98A3 },
    { 221, 0x9E3779B9, 0xD192AE46 },
    { 238, 0x00000000, 0x2DF87B0B },
    { 238, 0x9E3779B9, 0x8A9D9F97 },
    { 255, 0x00000000, 0xBFB8759B },
    { 255, 0x9E3779B9, 0xEF44B8AF },
    { 272, 0x00000000, 0x3FA97697 },
    { 272, 0x9E3779B9, 0xAB5391D9 },
};

int main()
{
    int errors = 0;

    for( int i = 0; i < sizeof(ref) / sizeof(ref[0]); ++i )
    {
        reference_entry const& e = ref[ i ];

        std::vector<unsigned char> v( e.length, 0x9C );

        std::uint32_t r = mulxp1_hash32( v.data(), v.size(), e.seed );

        if( r != e.result )
        {
            std::fprintf( stderr, "Reference value mismatch for length %zu, seed %08" PRIX32 ": was %08" PRIX32 ", expected %08" PRIX32 "\n", e.length, e.seed, r, e.result );
            ++errors;
        }
    }

    if( errors == 0 )
    {
        std::puts( "No errors detected." );
    }

    return errors;
}
