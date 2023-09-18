# Add all .cpp files from folder "src".
file(GLOB GLFONT_SOURCES "src/*.cpp")

# Add all .h files from folder "include/kgfw".
file(GLOB GLFONT_HEADERS "src/*.h")

# Create a library called kgfw.
add_library(glfont ${GLFONT_SOURCES} ${GLFONT_HEADERS})

# Add include directories to a target.
target_include_directories(glfont PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)