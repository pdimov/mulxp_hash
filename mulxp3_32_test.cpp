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
    {   0, 0x00000000, 0x8E655EAE },
    {   0, 0x9E3779B9, 0xEF63B598 },
    {  17, 0x00000000, 0x69931AE7 },
    {  17, 0x9E3779B9, 0xDC735952 },
    {  34, 0x00000000, 0xEBFAD8A5 },
    {  34, 0x9E3779B9, 0xD7B63531 },
    {  51, 0x00000000, 0xAA337B35 },
    {  51, 0x9E3779B9, 0x134C803C },
    {  68, 0x00000000, 0x42BABC15 },
    {  68, 0x9E3779B9, 0xB4A6F072 },
    {  85, 0x00000000, 0x1A39F5C2 },
    {  85, 0x9E3779B9, 0xAB805A44 },
    { 102, 0x00000000, 0xCA101728 },
    { 102, 0x9E3779B9, 0x2A3380E3 },
    { 119, 0x00000000, 0xD95677AE },
    { 119, 0x9E3779B9, 0xCE68A989 },
    { 136, 0x00000000, 0xD9E48D12 },
    { 136, 0x9E3779B9, 0xE4B7F350 },
    { 153, 0x00000000, 0x2C6040EB },
    { 153, 0x9E3779B9, 0x2277D67A },
    { 170, 0x00000000, 0x6B8F0E37 },
    { 170, 0x9E3779B9, 0x0F04D6F4 },
    { 187, 0x00000000, 0x6EA8D6AC },
    { 187, 0x9E3779B9, 0x06932376 },
    { 204, 0x00000000, 0x11568A19 },
    { 204, 0x9E3779B9, 0xB49F516B },
    { 221, 0x00000000, 0x733B857D },
    { 221, 0x9E3779B9, 0x15AD48B2 },
    { 238, 0x00000000, 0x9EF34228 },
    { 238, 0x9E3779B9, 0xD7444A15 },
    { 255, 0x00000000, 0x5ACF71B8 },
    { 255, 0x9E3779B9, 0x11A6CEF3 },
    { 272, 0x00000000, 0xFF645816 },
    { 272, 0x9E3779B9, 0x3C737157 },
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
