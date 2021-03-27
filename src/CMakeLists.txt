# Collecting module-specific source files
file(GLOB DATA_SRCS "dataLibrary/*.cpp")
file(GLOB GENERATOR_SRCS "generatorLibrary/*.cpp")
file(GLOB SEGMENTOR_SRCS "segmentorLibrary/*.cpp")

add_library(data ${DATA_SRCS})
add_library(generator ${GENERATOR_SRCS})
add_library(segmentor ${SEGMENTOR_SRCS})

# Headers targets indexing
set(TARGETS data;generator;segmentor)
foreach( TAR ${TARGETS} )
  target_include_directories( ${TAR}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    )
endforeach(TAR)
