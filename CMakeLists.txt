# Copyright 2023, 2024 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.14...3.25)

project(mulxp_test LANGUAGES CXX)

include(CTest)

function(mulxp_add_test name)

  add_executable(${name} ${name}.cpp)
  add_test(${name} ${name})

endfunction()

mulxp_add_test(mulxp1_test)
mulxp_add_test(mulxp3_test)
mulxp_add_test(mulxp1_32_test)
mulxp_add_test(mulxp3_32_test)
