include_directories(AFTER ${CONFUSE_INCLUDE_DIRS})

add_definitions(-DSRC_DIR=".")

add_executable(suite_single suite_single.c)
target_link_libraries(suite_single ${CONFUSE_LIBRARIES})
add_test(TestSuiteSingle suite_single)

add_executable(suite_dup suite_dup.c)
target_link_libraries(suite_dup ${CONFUSE_LIBRARIES})
add_test(TestSuiteDup suite_dup)

add_executable(suite_func suite_func.c)
target_link_libraries(suite_func ${CONFUSE_LIBRARIES})
add_test(TestSuiteFunc suite_func)

add_executable(suite_list suite_list.c)
target_link_libraries(suite_list ${CONFUSE_LIBRARIES})
add_test(TestSuiteList suite_list)

add_executable(suite_validate suite_validate.c)
target_link_libraries(suite_validate ${CONFUSE_LIBRARIES})
add_test(TestSuiteValidate suite_validate)

add_executable(list_plus_syntax list_plus_syntax.c)
target_link_libraries(list_plus_syntax ${CONFUSE_LIBRARIES})
add_test(TestListPlusSyntax list_plus_syntax)

add_executable(section_title_dupes section_title_dupes.c)
target_link_libraries(section_title_dupes ${CONFUSE_LIBRARIES})
add_test(TestSectionTitleDupes section_title_dupes)

add_executable(single_title_sections single_title_sections.c)
target_link_libraries(single_title_sections ${CONFUSE_LIBRARIES})
add_test(TestSingleTitleSections single_title_sections)

add_executable(quote_before_print quote_before_print.c)
target_link_libraries(quote_before_print ${CONFUSE_LIBRARIES})
add_test(TestQuoteBeforePrint quote_before_print)

add_executable(include include.c)
target_link_libraries(include ${CONFUSE_LIBRARIES})
add_test(TestInclude include)

add_executable(searchpath searchpath.c)
target_link_libraries(searchpath ${CONFUSE_LIBRARIES})
add_test(TestSearchPath searchpath)

add_executable(opts_array opts_array.c)
target_link_libraries(opts_array ${CONFUSE_LIBRARIES})
add_test(TestOptsArray opts_array)
