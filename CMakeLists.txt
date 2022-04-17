cmake_minimum_required (VERSION 2.8.7)
project (rulefinder)
set(ROOT_DIR ./)

add_subdirectory (src)
add_subdirectory (test)

# -lglib-2.0
#target_link_libraries(glib-2.0)
