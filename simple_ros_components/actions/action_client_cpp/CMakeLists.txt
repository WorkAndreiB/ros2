cmake_minimum_required(VERSION 3.8)
project(action_client_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(action_interfaces REQUIRED)
find_package(rclcpp_action REQUIRED)

add_library(add_until_client_cpp STATIC
    src/add_until_client.cpp
)

target_include_directories(add_until_client_cpp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(add_until_client_cpp rclcpp rclcpp_action action_interfaces)

add_executable(add_until_client src/main.cpp)

target_link_libraries(add_until_client add_until_client_cpp)

install(TARGETS add_until_client
  DESTINATION lib/${PROJECT_NAME})

ament_package()
