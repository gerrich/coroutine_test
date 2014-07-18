project(coroutines_test)
cmake_minimum_required(VERSION 2.8)

find_package( Boost 1.55.0 COMPONENTS system thread context coroutine regex date_time serialization )
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
    #"${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-unused-local-typedefs -stdlib=libc++")

add_executable(future_test future_test.cpp)
target_link_libraries(future_test
  ${Boost_CONTEXT_LIBRARY}
  ${Boost_COROUTINE_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
)

add_executable(xrange xrange.cpp)
target_link_libraries(xrange ${Boost_LIBRARIES})

add_executable(coro_test coro_test.cpp)
target_link_libraries(coro_test ${Boost_LIBRARIES})

add_executable(server server.cpp)
target_link_libraries(server ${Boost_LIBRARIES})

add_executable(daytime_client daytime_client.cpp)
target_link_libraries(daytime_client ${Boost_LIBRARIES})

add_executable(daytime_client_future daytime_client_future.cpp)
target_link_libraries(daytime_client_future ${Boost_LIBRARIES})

add_executable(daytime_client_coro daytime_client_coro.cpp)
target_link_libraries(daytime_client_coro ${Boost_LIBRARIES})
