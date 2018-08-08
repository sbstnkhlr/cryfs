include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(gitversion)
add_subdirectory(cpp-utils)
#add_subdirectory(fspp)
add_subdirectory(parallelaccessstore)
add_subdirectory(blockstore)
add_subdirectory(blobstore)
add_subdirectory(cryfs)
add_subdirectory(cryfs-cli)
#add_subdirectory(stats)

add_executable(test_executable test_executable.cpp)
target_link_libraries(test_executable cpp-utils)
