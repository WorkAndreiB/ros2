cmake_minimum_required(VERSION 3.8)
project(robot_arm_commander_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)

add_library(Commander STATIC src/commander.cpp)
target_include_directories(Commander PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(Commander rclcpp moveit_ros_planning_interface)

add_executable(MoveArm src/test_moveit.cpp)
target_link_libraries(MoveArm PRIVATE Commander)


install(TARGETS MoveArm
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

ament_package()
