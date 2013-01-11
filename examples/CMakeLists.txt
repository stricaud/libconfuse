include_directories(AFTER "${CONFUSE_INCLUDE_DIRS}")

add_executable(simple simple.c)
target_link_libraries(simple ${CONFUSE_LIBRARIES})

add_executable(reread reread.c)
target_link_libraries(reread ${CONFUSE_LIBRARIES})

add_executable(ftpconf ftpconf.c)
target_link_libraries(ftpconf ${CONFUSE_LIBRARIES})

if(WIN32)
add_executable(wincfgtest wincfgtest.c)
target_link_libraries(wincfgtest ${CONFUSE_LIBRARIES})
endif(WIN32)
