cmake_minimum_required(VERSION 3.8)
project(robot_arm_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(robot_arm_interfaces REQUIRED)

add_library(arm_controller STATIC
    src/robot_arm_controller.cpp
)

target_include_directories(arm_controller PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(arm_controller rclcpp rclcpp_action robot_arm_interfaces)

add_executable(robot_arm_controller src/main.cpp)

target_link_libraries(robot_arm_controller arm_controller)

ament_package()
