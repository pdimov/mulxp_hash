# mulxp_hash

Candidate replacements for use with `boost::unordered_flat_map` from
Boost 1.81, instead of the default `boost::hash<std::string>`. Might
be added as a dedicated `boost::string_hash` function object for
string hashing only, or directly as the implementation underneath
`boost::hash<std::string>` et al.

As a basic primitive, the functions use `mulx`, a multiplication of
two 64 bit numbers yielding a 128 bit result, then combining its two
64 parts with bitwise-xor. `mulx` has very good combiner properties
for its cost (a single `mul` instruction under x86), and has been
popularized by [Vladimir Makarov](https://github.com/vnmakarov) as
the basis of his hash function [MUM hash](https://github.com/vnmakarov/mum-hash).

`mulxp0_hash` is closest to `boost::hash<std::string>` from Boost 1.81,
and uses the same basic `state = combine( state, input );` round, with
`combine(h, v)` being `h ^ mulx( h + 1 + v, k )`, where `k` is a constant.
Unlike `boost::hash`, it adds a leading round of scrambling the initial
seed, and a trailing round of scrambling the final result. (These are
necessary to pass the
[SMHasher battery of tests](https://github.com/rurban/smhasher).)

Since this construction makes the next round dependent on the current one,
the loop is latency-bound. The `mul` instruction has higher throughput than
latency, which can be exploited by changing the round to
`state = combine( state, mix( input ) );`, with the `combine` function
cheaper than the `mix` one, a construct popularized by Austin Appleby's
[MurmurHash](https://github.com/aappleby/smhasher/blob/61a0530f28277f2e850bfc39600ce61d02b518de/src/MurmurHash2.cpp).

`mulxp1_hash` follows this approach, with a round of
`h ^= mulx( v1 + w, k );`, where `k` is, as before, a constant, and `w`
is a [Weyl sequence](https://en.wikipedia.org/wiki/Weyl_sequence) (a
fancy way of saying that `w` is updated on each iteration by adding a
constant to it, `w += q;`.)

On sufficiently long inputs, where the parts outside the loop don't matter,
`mulxp1_hash` is about 2.5 times as fast as `mulxp0_hash`. (However, long
inputs don't really matter for our primary target use case, a hash function
for `boost::unordered_flat_map<std::string, X>`, as most keys are small.)

`mulxp2_hash` is a slight variation of `mulxp1_hash`, using the same round,
which basically unrolls the main loop twice, consuming 16 bytes at a time.
This makes it slightly (10%) faster on long inputs, but slower on short
inputs.

The final member of the `mulxp` family is `mulxp3_hash`. It has been inspired
by the [UMAC/NH](https://en.wikipedia.org/wiki/UMAC) family of functions
started by [FARSH](https://github.com/Bulat-Ziganshin/FARSH), continued with
[MUM](https://github.com/vnmakarov/mum-hash) and perfected with
[wyhash](https://github.com/wangyi-fudan/wyhash). These consume 16 bytes
(two words) of input at a time, `v1` and `v2`, and feed both to the `mulx`
primitive, as in `mulx(v1 + k1, v2 + k2)`, where `k1` and `k2` are suitably
chosen constants that ideally would come from a secret one-time pad.

The advantage of this scheme is that the inner loop performs one fewer
multiplication per round, which ideally would double the speed. The
disadvantage is that if one of the operands to the `mulx` primitive happens
to be zero, the other input word is essentially discarded and does not affect
the final hash value.

In `mulxp3_hash`, the round operation is `h ^= mulx( v1 + w, v2 + w + k );`,
where `w` is a Weyl sequence as above, and `k` is a constant. This makes
`mulxp3_hash` approximately 1.5 times as fast as `mulxp2_hash`.

## SMHasher Results

Results from running [rurban/smhasher](https://github.com/rurban/smhasher):

### mulxp0_hash

```
Verification value 0xBE132908 ....... PASS

Average      -  1.433 bytes/cycle - 4100.53 MiB/sec @ 3 ghz
Average                                    28.318 cycles/hash

Verification value is 0x00000001 - Testing took 1777.521000 seconds
```

No test failures.

### mulxp1_hash

```
Verification value 0xA476BA89 ....... PASS

Average      -  3.734 bytes/cycle - 10682.90 MiB/sec @ 3 ghz
Average                                    23.890 cycles/hash

Verification value is 0x00000001 - Testing took 1664.050000 seconds
```

No test failures.

### mulxp2_hash

```
Verification value 0x85C9262A ....... PASS

Average      -  4.061 bytes/cycle - 11619.43 MiB/sec @ 3 ghz
Average                                    26.145 cycles/hash

Verification value is 0x00000001 - Testing took 1667.155000 seconds
```

No test failures.

### mulxp3_hash

```
Verification value 0xB4786096 ....... PASS

Average      -  5.921 bytes/cycle - 16941.48 MiB/sec @ 3 ghz
Average                                    21.886 cycles/hash

Verification value is 0x00000001 - Testing took 1641.350000 seconds
```

No test failures.

### boost_hash

```
Verification value 0x8766204A ....... PASS

Average      -  0.377 bytes/cycle - 1077.76 MiB/sec @ 3 ghz
Average                                    58.984 cycles/hash

Verification value is 0x00000001 - Testing took 1976.459000 seconds
```

Failures: PerlinNoise

```
Testing 16777216 coordinates (L2) :
Testing collisions ( 64-bit) - Expected    0.0, actual 16769025 (2197949775808.02x) (16769025) !!!!!
Testing collisions (high 32-bit) - Expected      32725.4, actual 16769025 (512.42x) (16736300) !!!!!
Testing collisions (high 27-42 bits) - Worst is 42 bits: 16769025/31 (524032.73x) !!!!!
Testing collisions (low  32-bit) - Expected      32725.4, actual 16769025 (512.42x) (16736300) !!!!!
Testing collisions (low  27-42 bits) - Worst is 42 bits: 16769025/31 (524032.73x) !!!!!

Testing AV variant, 128 count with 4 spacing, 4-12:
Testing collisions ( 64-bit) - Expected    0.0, actual 674604 (2595263322583.69x) (674604)
Testing collisions (high 32-bit) - Expected       1116.2, actual 675248 (604.98x) (674132) !!!!!
Testing collisions (high 25-37 bits) - Worst is 37 bits: 674627/34 (19337.02x) !!!!!
Testing collisions (low  32-bit) - Expected       1116.2, actual 675267 (605.00x) (674151) !!!!!
Testing collisions (low  25-37 bits) - Worst is 37 bits: 674622/34 (19336.88x) !!!!!
```

## Benchmark Results

Results from running the
[`unordered_flat.cpp` benchmark](https://github.com/boostorg/container_hash/blob/16546190f6f441b8c341d3887120d2f687369428/benchmark/unordered_flat.cpp)
from Boost.ContainerHash, modified to test the `mulxp` family of functions
in addition to `boost::hash<std::string>` and `absl::Hash<std::string>`.

### GCC 11 -O3

```
         boost::hash: 14990 ms
          absl::Hash: 12352 ms
         mulxp0_hash: 11533 ms
         mulxp1_hash: 11716 ms
         mulxp2_hash: 11800 ms
         mulxp3_hash: 11101 ms
```

### clang-cl 15 /O2

```
         boost::hash: 17352 ms
          absl::Hash: 14732 ms
         mulxp0_hash: 13479 ms
         mulxp1_hash: 13589 ms
         mulxp2_hash: 14673 ms
         mulxp3_hash: 13962 ms
```

### VS2022 /O2 /GL

```
         boost::hash: 19053 ms
          absl::Hash: 16200 ms
         mulxp0_hash: 13496 ms
         mulxp1_hash: 13651 ms
         mulxp2_hash: 14072 ms
         mulxp3_hash: 13213 ms
```
