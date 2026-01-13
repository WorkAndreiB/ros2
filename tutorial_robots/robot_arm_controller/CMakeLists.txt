cmake_minimum_required(VERSION 3.8)
project(robot_arm_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(robot_arm_commander_cpp REQUIRED)
find_package(robot_arm_interfaces REQUIRED)

add_library(ArmControllerLib STATIC src/controller.cpp)
target_include_directories(ArmControllerLib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(ArmControllerLib 
    rclcpp 
    example_interfaces 
    robot_arm_commander_cpp 
    robot_arm_interfaces
)

add_executable(ArmControllerExec src/main.cpp)
target_link_libraries(ArmControllerExec 
    PRIVATE 
    ArmControllerLib
)

install(TARGETS ArmControllerExec
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

ament_package()
