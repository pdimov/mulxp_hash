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
    std::uint64_t seed;
    std::uint32_t result;
};

reference_entry const ref[] =
{
    {   0, 0x0000000000000000, 0x6F5D607C },
    {   0, 0x000000009E3779B9, 0x49DAC063 },
    {   0, 0x9E3779B97F4A7C15, 0x91524C8F },
    {  17, 0x0000000000000000, 0xA9830593 },
    {  17, 0x000000009E3779B9, 0xA3ED9324 },
    {  17, 0x9E3779B97F4A7C15, 0xE9AA0CB5 },
    {  34, 0x0000000000000000, 0x580A9DFB },
    {  34, 0x000000009E3779B9, 0x79334B61 },
    {  34, 0x9E3779B97F4A7C15, 0xF180A2F1 },
    {  51, 0x0000000000000000, 0xEC81A87C },
    {  51, 0x000000009E3779B9, 0x089B2005 },
    {  51, 0x9E3779B97F4A7C15, 0xA677AE59 },
    {  68, 0x0000000000000000, 0x044EB1FA },
    {  68, 0x000000009E3779B9, 0xA9E373FA },
    {  68, 0x9E3779B97F4A7C15, 0xE3C240A5 },
    {  85, 0x0000000000000000, 0xBD925846 },
    {  85, 0x000000009E3779B9, 0x9C636C64 },
    {  85, 0x9E3779B97F4A7C15, 0x02933DE4 },
    { 102, 0x0000000000000000, 0x1D6857EA },
    { 102, 0x000000009E3779B9, 0x57866FC5 },
    { 102, 0x9E3779B97F4A7C15, 0xD1ADC02C },
    { 119, 0x0000000000000000, 0xB1F4C4A5 },
    { 119, 0x000000009E3779B9, 0x70C6F1E3 },
    { 119, 0x9E3779B97F4A7C15, 0xAF8C7828 },
    { 136, 0x0000000000000000, 0x2F519887 },
    { 136, 0x000000009E3779B9, 0x26E25BC8 },
    { 136, 0x9E3779B97F4A7C15, 0x801ECE05 },
    { 153, 0x0000000000000000, 0x4923B36C },
    { 153, 0x000000009E3779B9, 0x2E25C187 },
    { 153, 0x9E3779B97F4A7C15, 0xFFA4C1AD },
    { 170, 0x0000000000000000, 0xB9FF9CB5 },
    { 170, 0x000000009E3779B9, 0xC0A36E75 },
    { 170, 0x9E3779B97F4A7C15, 0x5C256B00 },
    { 187, 0x0000000000000000, 0x9DC48EA3 },
    { 187, 0x000000009E3779B9, 0x29C12DF6 },
    { 187, 0x9E3779B97F4A7C15, 0xF98121BE },
    { 204, 0x0000000000000000, 0x42495C84 },
    { 204, 0x000000009E3779B9, 0x4A32B5E1 },
    { 204, 0x9E3779B97F4A7C15, 0xFB117D56 },
    { 221, 0x0000000000000000, 0x1A1E98A3 },
    { 221, 0x000000009E3779B9, 0xD192AE46 },
    { 221, 0x9E3779B97F4A7C15, 0x58707551 },
    { 238, 0x0000000000000000, 0x2DF87B0B },
    { 238, 0x000000009E3779B9, 0x8A9D9F97 },
    { 238, 0x9E3779B97F4A7C15, 0xB3647B31 },
    { 255, 0x0000000000000000, 0xBFB8759B },
    { 255, 0x000000009E3779B9, 0xEF44B8AF },
    { 255, 0x9E3779B97F4A7C15, 0x0305D1F0 },
    { 272, 0x0000000000000000, 0x3FA97697 },
    { 272, 0x000000009E3779B9, 0xAB5391D9 },
    { 272, 0x9E3779B97F4A7C15, 0x8248DE41 },
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
            std::fprintf( stderr, "Reference value mismatch for length %zu, seed %016" PRIX64 ": was %08" PRIX32 ", expected %08" PRIX32 "\n", e.length, e.seed, r, e.result );
            ++errors;
        }
    }

    if( errors == 0 )
    {
        std::puts( "No errors detected." );
    }

    return errors;
}
