include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(tests  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)#EXCLUDE_FROM_ALL
target_link_libraries(tests ${PROJECT_NAME} ${PERL_LINKER_FLAGS})
#target_link_libraries(tests ${PROJECT_NAME}  )

set(OTHER_FILES modules.pl)
add_custom_target(dummy ${OTHER_FILES})
