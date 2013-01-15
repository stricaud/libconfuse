include_directories(AFTER "include/")

FLEX_TARGET(Scanner lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c COMPILE_FLAGS "-Pcfg_yy")

set(CONFUSE_SRC_FILES
  confuse.c
  options.c
  opts_array.c
  portable.c
  ${FLEX_Scanner_OUTPUTS}
)

add_library(confuse SHARED ${CONFUSE_SRC_FILES})
set_target_properties(confuse PROPERTIES SOVERSION 1)
target_link_libraries(confuse ${FLEX_LIBRARIES})

install(TARGETS confuse LIBRARY DESTINATION lib)

