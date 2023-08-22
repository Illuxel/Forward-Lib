@PACKAGE_INIT@

include(${CMAKE_CURRENT_LIST_DIR}/fl-targets.cmake)
include(CMakeFindDependencyMacro)

find_dependency(OpenSSL)
find_dependency(Boost)
# find_dependency(jwt-cpp)
# find_dependency(SQLite)