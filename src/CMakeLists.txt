set(bitzer_SRCS
    main.c
    log.c
    strutil.c
    sighandler.c
    context.c
    rbtree.c
    task.c
    alloc.c
)

add_custom_command(
    SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/conf_scan.l
    COMMAND ${FLEX_EXECUTABLE}
    ARGS -o${CMAKE_CURRENT_BINARY_DIR}/conf_scan.c
           ${CMAKE_CURRENT_SOURCE_DIR}/conf_scan.l
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/conf_scan.c
    )

add_custom_command(
    SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/conf_gram.y
    COMMAND ${BISON_EXECUTABLE}
    ARGS -d -o ${CMAKE_CURRENT_BINARY_DIR}/conf_gram.c
            ${CMAKE_CURRENT_SOURCE_DIR}/conf_gram.y
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/conf_scan.c
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/conf_gram.c
    )

set(bitzer_SRCS ${bitzer_SRCS}
    ${CMAKE_CURRENT_BINARY_DIR}/conf_scan.c
    ${CMAKE_CURRENT_BINARY_DIR}/conf_gram.c
   )

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/conf_scan.c PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/conf_gram.c PROPERTIES GENERATED TRUE)

add_executable(bitzer ${bitzer_SRCS})

add_custom_target(bitzerconf DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/conf_gram.c)

add_dependencies(bitzer bitzerconf)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS bitzer DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

