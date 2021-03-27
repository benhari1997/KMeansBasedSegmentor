add_executable(appLauncher src/main.cpp)

# Libraries source linking
target_link_libraries( appLauncher
  PUBLIC ${OpenCV_LIBS}
  PUBLIC data
  PUBLIC generator
  PUBLIC segmentor
  )
