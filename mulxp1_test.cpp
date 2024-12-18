// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "mulxp_hash.hpp"
#include <vector>
#include <cstdio>
#include <cinttypes>

// Test vectors for mulxp1_hash

struct reference_entry
{
    std::size_t length;
    std::uint64_t seed;
    std::uint64_t result;
};

reference_entry const ref[] =
{
    {   0, 0x0000000000000000, 0x1C97DA6F7F24EBA6 },
    {   0, 0x000000009E3779B9, 0x51BDC03984425EB7 },
    {   0, 0x9E3779B97F4A7C15, 0x7728005A58BB1B47 },
    {  17, 0x0000000000000000, 0xDEBC51778DD91488 },
    {  17, 0x000000009E3779B9, 0x325B4BEA16D09575 },
    {  17, 0x9E3779B97F4A7C15, 0xC43CBAE27255479E },
    {  34, 0x0000000000000000, 0x2BB78778295CD3D2 },
    {  34, 0x000000009E3779B9, 0x60186FF61453E3A3 },
    {  34, 0x9E3779B97F4A7C15, 0x5EB1F10F73FE70AA },
    {  51, 0x0000000000000000, 0x1AFEB63C8AAE2E48 },
    {  51, 0x000000009E3779B9, 0x8D76E8A925788723 },
    {  51, 0x9E3779B97F4A7C15, 0x3124AF349FED5035 },
    {  68, 0x0000000000000000, 0x5BA86803557780A1 },
    {  68, 0x000000009E3779B9, 0x429EA61BFF709526 },
    {  68, 0x9E3779B97F4A7C15, 0x9EBE025BEC7B8F88 },
    {  85, 0x0000000000000000, 0xC51A8F4284C1B9F1 },
    {  85, 0x000000009E3779B9, 0x70B49B47E7EF71DD },
    {  85, 0x9E3779B97F4A7C15, 0x5013DB821469205F },
    { 102, 0x0000000000000000, 0x22DCBAA741B97A0B },
    { 102, 0x000000009E3779B9, 0x18EAB7540C5E7AF7 },
    { 102, 0x9E3779B97F4A7C15, 0x45AA8F1C8D6C85DE },
    { 119, 0x0000000000000000, 0x58FF9B0F9CE8D41E },
    { 119, 0x000000009E3779B9, 0x0C192C0D099137A0 },
    { 119, 0x9E3779B97F4A7C15, 0xD743AD0FDEEB9F0B },
    { 136, 0x0000000000000000, 0x56BE5124B008605D },
    { 136, 0x000000009E3779B9, 0xFD34F2560C610087 },
    { 136, 0x9E3779B97F4A7C15, 0xA6664F1A9D45D45E },
    { 153, 0x0000000000000000, 0x78B2A761C4FC343E },
    { 153, 0x000000009E3779B9, 0xDABD744401EDCB72 },
    { 153, 0x9E3779B97F4A7C15, 0x5EFF81FE3AC47A1B },
    { 170, 0x0000000000000000, 0x066FCFF2E49E6CAF },
    { 170, 0x000000009E3779B9, 0xA30B1D473CD6DBA4 },
    { 170, 0x9E3779B97F4A7C15, 0xDD7C87D691EE7AFF },
    { 187, 0x0000000000000000, 0x9FE11A4D8F9B4365 },
    { 187, 0x000000009E3779B9, 0x80BE5E8912B81194 },
    { 187, 0x9E3779B97F4A7C15, 0x610431AE251CEC8B },
    { 204, 0x0000000000000000, 0x8A56C2026BFCC344 },
    { 204, 0x000000009E3779B9, 0x47271D582D54FCFD },
    { 204, 0x9E3779B97F4A7C15, 0x4D4B0F722C9EFACB },
    { 221, 0x0000000000000000, 0x1FEE1669F3BE0DAE },
    { 221, 0x000000009E3779B9, 0x1EADD262E7E2C8D2 },
    { 221, 0x9E3779B97F4A7C15, 0xC0A9B4FB0301623C },
    { 238, 0x0000000000000000, 0x8B332C7D8A1DAB7F },
    { 238, 0x000000009E3779B9, 0x4973EFB3A14BEB94 },
    { 238, 0x9E3779B97F4A7C15, 0xBF78C4B4D032BBC6 },
    { 255, 0x0000000000000000, 0x651957B949FB304C },
    { 255, 0x000000009E3779B9, 0x9133DBEE15F361E1 },
    { 255, 0x9E3779B97F4A7C15, 0x80BD6CEF705B2C84 },
    { 272, 0x0000000000000000, 0xFF49BF87CBA392E2 },
    { 272, 0x000000009E3779B9, 0x623F71308C2CECD5 },
    { 272, 0x9E3779B97F4A7C15, 0x7B174B440D2E8DA9 },
};

int main()
{
    int errors = 0;

    for( int i = 0; i < sizeof(ref) / sizeof(ref[0]); ++i )
    {
        reference_entry const& e = ref[ i ];

        std::vector<unsigned char> v( e.length, 0x9C );

        std::uint64_t r = mulxp1_hash( v.data(), v.size(), e.seed );

        if( r != e.result )
        {
            std::fprintf( stderr, "Reference value mismatch for length %zu, seed %016" PRIX64 ": was %016" PRIX64 ", expected %016" PRIX64 "\n", e.length, e.seed, r, e.result );
            ++errors;
        }
    }

    if( errors == 0 )
    {
        std::puts( "No errors detected." );
    }

    return errors;
}
