add_executable(prng_tst testPrNG.cpp)
# add_executable(generator_tst testGenerator.cpp)
add_executable(generate_tst testGenerateMethod.cpp)

# Shared headers targets indexing
set(TARGETS
  prng_tst;
  generate_tst
  # generator_tst
  )
foreach( TAR ${TARGETS} )
  target_link_libraries(${TAR} PRIVATE gmock_main data generator)
  target_include_directories(${TAR} PRIVATE ${PROJECT_SOURCE_DIR}/include)
endforeach(TAR)

# Individual headers targets indexing

# Test cases declaration
add_test(
  NAME prng
  COMMAND prng_tst
  )

# add_test(
#   NAME generator
#   COMMAND generator_tst
#   )

add_test(
  NAME generate
  COMMAND generate_tst
  )
