cmake_minimum_required(VERSION 3.8)
project(action_server_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(action_interfaces REQUIRED)
find_package(rclcpp_action REQUIRED)

add_executable(add_until_server src/add_until_server.cpp)
ament_target_dependencies(add_until_server rclcpp rclcpp_action action_interfaces)

install(TARGETS add_until_server
  DESTINATION lib/${PROJECT_NAME})

ament_package()
