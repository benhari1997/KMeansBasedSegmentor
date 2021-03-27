add_executable(atomic_tst testAtomic.cpp)
add_executable(container_tst testContainer.cpp)

# Shared headers targets indexing
set(TARGETS
  atomic_tst;
  container_tst
  )
foreach( TAR ${TARGETS} )
  target_link_libraries(${TAR} PRIVATE gmock_main data)
  target_include_directories(${TAR} PRIVATE ${PROJECT_SOURCE_DIR}/include)
endforeach(TAR)

# Individual headers targets indexing

# Test cases declaration
add_test(
  NAME atomic
  COMMAND atomic_tst
  )

add_test(
  NAME container
  COMMAND container_tst
  )
