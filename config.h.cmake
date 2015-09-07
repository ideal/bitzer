#ifndef BITZER_CONFIG_H
#define BITZER_CONFIG_H

#define BITZER_VERSION "${BITZER_VERSION}"
#define BITZER_NAME    "bitzer"

#define BZ_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}"
#cmakedefine BZ_LOG_PATH  "${LOG_PATH}"
#cmakedefine BZ_CONF_PATH "${CONF_PATH}"
#cmakedefine BZ_PID_PATH  "${PID_PATH}"

#cmakedefine BZ_DEBUG_LOG

#cmakedefine HAVE_BACKTRACE
#cmakedefine HAVE_BACKTRACE_SYMBOLS
#cmakedefine HAVE_POSIX_MEMALIGN
#cmakedefine HAVE_MEMALIGN
#cmakedefine HAVE_GETOPT_LONG
#cmakedefine HAVE_GETIFADDRS
#cmakedefine HAVE_PSELECT

#endif
