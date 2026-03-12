cmake_minimum_required(VERSION 3.8)
project(action_server_py)

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)

find_package(rclpy REQUIRED)
find_package(action_interfaces REQUIRED)

ament_python_install_package(${PROJECT_NAME})

install(PROGRAMS
  action_server_py/add_until_server.py
  DESTINATION lib/${PROJECT_NAME}
  RENAME add_until_server
)

if(BUILD_TESTING)
  find_package(launch_testing_ament_cmake REQUIRED)

  add_launch_test(test/test_parallel_policy.py)
endif()

ament_package()