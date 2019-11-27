# - Try to find MySQL
#
# The following variables are optionally searched for defaults
#  MySQL_ROOT_DIR:            Base directory where all GLOG components are found
#
# The following are set after configuration is done:
#  MySQL_FOUND
#  MySQL_INCLUDE_DIRS
#  MySQL_LIBRARIES



set(MySQL_ROOT_DIR "" CACHE PATH "Folder contains MySQL Server")


find_path(MySQL_INCLUDE_DIR mysql.h
        HINTS $ENV{MYSQL_HOME}
        PATH_SUFFIXES include)

find_library(MySQL_LIBRARY libmysql
        HINTS $ENV{MYSQL_HOME}
        PATH_SUFFIXES lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MySQL DEFAULT_MSG MySQL_INCLUDE_DIR MySQL_LIBRARY)

if(MySQL_FOUND)
	  set(MySQL_INCLUDE_DIRS ${MySQL_INCLUDE_DIR})
	  set(MySQL_LIBRARIES ${MySQL_LIBRARY})
	  message(STATUS "Found MySQL    (include: ${MySQL_INCLUDE_DIR}, library: ${MySQL_LIBRARY})")
	  mark_as_advanced(MySQL_ROOT_DIR MySQL_LIBRARY MySQL_INCLUDE_DIR)
endif()
