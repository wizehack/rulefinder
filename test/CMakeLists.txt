set(ROOT_DIR ../)
include_directories(${ROOT_DIR}/)
include_directories(${ROOT_DIR}/include)
include_directories(${ROOT_DIR}/test/include)
#-Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -std=c++11

# Build to Test
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++ -O0 -g -Wall -std=c++11 -pthread")

# Build to meassure Coverage
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -Weffc++ -g -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/local/include/json-c -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -Weffc++ -g -Wall -std=c++11 -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/bin/")

add_executable(
    test
    ${ROOT_DIR}/test/src/OneshotRuleTestHelper.cpp
    ${ROOT_DIR}/test/src/CompositRuleTestHelper.cpp
    ${ROOT_DIR}/test/src/BoolExprTestHelper.cpp
    ${ROOT_DIR}/test/src/testmain.cpp
    ${ROOT_DIR}/src/boolEval.cpp
    ${ROOT_DIR}/src/oneshotRuleAnalysis.cpp
    ${ROOT_DIR}/src/compositeRuleAnalysis.cpp
)

# -lglib-2.0
#target_link_libraries(glib-2.0)
