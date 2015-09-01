set(bitzer_SRCS
    main.c
    log.c
    strutil.c
    sighandler.c
    context.c
    rbtree.c
)

add_executable(bitzer ${bitzer_SRCS})

install(TARGETS bitzer DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

