add_executable(cluster_tst testCluster.cpp)
add_executable(segmentor_tst testSegmentor.cpp)
add_executable(segmentor_pxl_tst testSegmentorPixel.cpp)

# Shared headers targets indexing
set(TARGETS
    cluster_tst;
    segmentor_tst;
    segmentor_pxl_tst;
  )
foreach( TAR ${TARGETS} )
  target_link_libraries(${TAR} PRIVATE gmock_main data segmentor)
  target_include_directories(${TAR} PRIVATE ${PROJECT_SOURCE_DIR}/include)
endforeach(TAR)

# Individual headers targets indexing
target_link_libraries(segmentor_pxl_tst PUBLIC ${OpenCV_LIBS})

# Test cases declaration
add_test(
  NAME cluster
  COMMAND cluster_tst
  )
add_test(
  NAME segmentor
  COMMAND segmentor_tst
)
add_test(
  NAME segmentor_pxl
  COMMAND segmentor_pxl_tst
)