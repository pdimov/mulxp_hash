// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "mulxp_hash.hpp"
#include <vector>
#include <cstdio>

// Test vectors for mulxp3_hash

struct reference_entry
{
    std::size_t length;
    std::uint64_t seed;
    std::uint64_t result;
};

reference_entry const ref[] =
{
    {   0, 0x0000000000000000, 0xC814FCA3D94F2D9D },
    {   0, 0x000000009E3779B9, 0x8F790749C68256EE },
    {   0, 0x9E3779B97F4A7C15, 0x100135371D77D2E0 },
    {  17, 0x0000000000000000, 0x5E3490480D57F9E9 },
    {  17, 0x000000009E3779B9, 0x7B677C70534372FC },
    {  17, 0x9E3779B97F4A7C15, 0xB4EDBDEB2D3D92F3 },
    {  34, 0x0000000000000000, 0xC746ADC8A63A4706 },
    {  34, 0x000000009E3779B9, 0x2F77B7B2E6ADA87A },
    {  34, 0x9E3779B97F4A7C15, 0x0ABF9EB2F7A1C79C },
    {  51, 0x0000000000000000, 0xC6EE9C1504815F77 },
    {  51, 0x000000009E3779B9, 0x62C47698A815540B },
    {  51, 0x9E3779B97F4A7C15, 0x058BB7486A7C3120 },
    {  68, 0x0000000000000000, 0x371B2CA8CE900A3E },
    {  68, 0x000000009E3779B9, 0x905B2A4C75D71C44 },
    {  68, 0x9E3779B97F4A7C15, 0xE7335ED7FC62CC30 },
    {  85, 0x0000000000000000, 0x2909D55C4E276AF3 },
    {  85, 0x000000009E3779B9, 0x5AC80FE851CBAEC4 },
    {  85, 0x9E3779B97F4A7C15, 0x9535FDF19D6D8D5C },
    { 102, 0x0000000000000000, 0x1D965BD19075E377 },
    { 102, 0x000000009E3779B9, 0xC7D2849DF10C4853 },
    { 102, 0x9E3779B97F4A7C15, 0xE5150A034928991E },
    { 119, 0x0000000000000000, 0xC8898B9959253496 },
    { 119, 0x000000009E3779B9, 0x910357D7BB7A1870 },
    { 119, 0x9E3779B97F4A7C15, 0xB440740C5F0CF770 },
    { 136, 0x0000000000000000, 0x8BE5DCF4366C7F7C },
    { 136, 0x000000009E3779B9, 0x1BD293C4EF0D84ED },
    { 136, 0x9E3779B97F4A7C15, 0x197B8C424C12F6CC },
    { 153, 0x0000000000000000, 0xD2F4DE0FCEA3A349 },
    { 153, 0x000000009E3779B9, 0x896FB2D39FB7EFA7 },
    { 153, 0x9E3779B97F4A7C15, 0x963FA900E393DF5F },
    { 170, 0x0000000000000000, 0x2947A0DF25D037E1 },
    { 170, 0x000000009E3779B9, 0xD2FA104770E9B6A9 },
    { 170, 0x9E3779B97F4A7C15, 0xA78BD679580E9BD0 },
    { 187, 0x0000000000000000, 0xA8D2977F35CAD60E },
    { 187, 0x000000009E3779B9, 0xD9141E18F2D9ACA2 },
    { 187, 0x9E3779B97F4A7C15, 0xC727D6B05516DDCB },
    { 204, 0x0000000000000000, 0x3875A65033134A3A },
    { 204, 0x000000009E3779B9, 0x3EAE20AAFEA1531E },
    { 204, 0x9E3779B97F4A7C15, 0xEF1095EEB8705021 },
    { 221, 0x0000000000000000, 0x3F9CC647D7341AA1 },
    { 221, 0x000000009E3779B9, 0x632EE74082F74872 },
    { 221, 0x9E3779B97F4A7C15, 0x335FD82FF23FCF24 },
    { 238, 0x0000000000000000, 0xAB509AF2B5D2EDC1 },
    { 238, 0x000000009E3779B9, 0xD939C11E09589C49 },
    { 238, 0x9E3779B97F4A7C15, 0x58A6BEA2BC9DC61B },
    { 255, 0x0000000000000000, 0x4FE4BE6884AF8E2A },
    { 255, 0x000000009E3779B9, 0x3E22D2D3DA7251C4 },
    { 255, 0x9E3779B97F4A7C15, 0x9FA0D7C0817E2345 },
    { 272, 0x0000000000000000, 0x087DC6E88BC4432A },
    { 272, 0x000000009E3779B9, 0x2D9EA73EBF915613 },
    { 272, 0x9E3779B97F4A7C15, 0x0805FC8A45A8B483 },
};

int main()
{
    int errors = 0;

    for( int i = 0; i < sizeof(ref) / sizeof(ref[0]); ++i )
    {
        reference_entry const& e = ref[ i ];

        std::vector<unsigned char> v( e.length, 0x9C );

        std::uint64_t r = mulxp3_hash( v.data(), v.size(), e.seed );

        if( r != e.result )
        {
            std::fprintf( stderr, "Reference value mismatch for length %zu, seed %016llX: was %016llX, expected %016llX\n", e.length, e.seed, r, e.result );
            ++errors;
        }
    }

    if( errors == 0 )
    {
        std::puts( "No errors detected." );
    }

    return errors;
}
